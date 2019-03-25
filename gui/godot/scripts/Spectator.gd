# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Sacha Delanoue

extends Node

const DumpReader = preload("res://scripts/DumpReader.gd")

var socket = null
var waiting = false
var playing = false
var animating = false
var turn_index = 0
var actions_playing = []
var my_stechec_id = -1
var my_internal_id = -1
var interactive = false
var my_turn = false

func _init_socket():
	var port = 0
	for arg in OS.get_cmdline_args():
		if arg.begins_with("-socket="):
			port = int(arg.right(8))
		elif arg.begins_with("-id="):
			my_stechec_id = int(arg.right(4))
	socket = StreamPeerTCP.new()
	var connected = socket.connect_to_host("127.0.0.1", port)
	if connected != OK:
		print("Could not connect")

func _ready():
	_init_socket()
	var available = 0
	while available == 0:
		available = socket.get_available_bytes()
	var dump = socket.get_string(available)
	var json = JSON.parse(dump).result
	if not json:
		print("invalid json ", dump)
	if json["players"].has(str(my_stechec_id)):
		interactive = true
		my_internal_id = 0
		for id in json["players"].keys():
			if int(id) < my_stechec_id:
				my_internal_id = 1
	var init = DumpReader.parse_turn(json)
	$GameState.init(init.walls, init.players[0].agents + init.players[1].agents)
	for agent in $GameState/TileMap.agents:
		 agent.connect("finished_moving", self, "_finish_animating")
	$GameState/Info.players[0].name = init.players[0].name
	$GameState/Info.players[1].name = init.players[1].name
	for alien_input in init.aliens:
		var alien = $GameState/TileMap.Alien.new()
		alien.pos = alien_input.pos
		alien.points = alien_input.points
		alien.first_turn = alien_input.first_turn
		alien.duration = alien_input.duration
		alien.capture = alien_input.capture
		$GameState/TileMap.aliens.append(alien)
	$GameState.set_turn(0)
	if interactive:
		$GameState.set_turn(1)
		$GameState.select_agent(my_internal_id * constants.NB_AGENTS)
		playing = true
		socket.put_utf8_string("NEXT")
		waiting = true

func _finish_animating():
	animating = false

var _future_state = null

func _next_turn():
	if my_turn:
		socket.put_utf8_string("NEXT")
		waiting = true
		my_turn = false
		$GameState.set_turn(turn_index + 1 + my_internal_id)
		return
	turn_index += 1
	$GameState.set_turn(turn_index)
	my_turn = false
	if turn_index % 3:
		if interactive:
			if turn_index % 3 - 1 == my_internal_id:
				my_turn = true
			else:
				actions_playing = _future_state.players[turn_index % 3 - 1].history
		else:
			socket.put_utf8_string("NEXT")
			waiting = true
	elif interactive:
		for i in range(_future_state.aliens.size()):
			$GameState/TileMap.aliens[i].capture = _future_state.aliens[i].capture
			assert((turn_index - turn_index % 3) / 3 == _future_state.roundNumber)
			$GameState.set_turn(turn_index) # To update the aliens
			for player_id in range(2):
				$GameState/Info.players[player_id].score = _future_state.players[player_id].score

func _process(delta):
	if waiting:
		var available = socket.get_available_bytes()
		if available and not interactive:
			var dump = socket.get_string(available)
			var json = JSON.parse(dump).result
			var state = DumpReader.parse_turn(json)
			for i in range(state.aliens.size()):
				$GameState/TileMap.aliens[i].capture = state.aliens[i].capture
			assert((turn_index - turn_index % 3) / 3 == state.roundNumber)
			$GameState.set_turn(turn_index) # To update the aliens
			actions_playing = state.players[turn_index % 3 - 1].history
			for player_id in range(2):
				$GameState/Info.players[player_id].score = state.players[player_id].score
			waiting = false
		elif available and interactive:
			var json = JSON.parse(socket.get_string(available)).result
			_future_state = DumpReader.parse_turn(json)
			waiting = false
			_next_turn()
	while not animating and actions_playing:
		animating = $GameState.replay_action(actions_playing.pop_front(), turn_index % 3 - 1)
	if playing and not actions_playing and not waiting and not my_turn:
		_next_turn()
	if Input.is_action_just_pressed("ui_select"):
		playing = !playing or interactive
		if my_turn:
			_next_turn()
	if not playing and not actions_playing and not my_turn and Input.is_action_just_pressed("ui_right"):
		_next_turn()
	elif my_turn and Input.is_action_just_pressed("ui_cancel"):
		$GameState.undo()
		socket.put_utf8_string("UNDO")
	$Waiting.set_visible(waiting)

func _action(pos):
	var agent_selected = $GameState.selected_agent
	if agent_selected < constants.NB_AGENTS * my_internal_id or \
			agent_selected >= constants.NB_AGENTS * (1 + my_internal_id):
		return
	agent_selected -= constants.NB_AGENTS * my_internal_id
	var offset = my_internal_id * constants.NB_AGENTS
	var agent_pos = $GameState/TileMap.agents_pos[agent_selected + offset]
	var dx = pos.x - agent_pos.x
	var dy = pos.y - agent_pos.y
	# One of them must be zero
	if dx != 0 and dy != 0:
		return
	if dx == 0 and dy == 0:
		return
	var dir = 0
	if dy > 0:
		dir = 1
	elif dx > 0:
		dir = 2
	elif dy < 0:
		dir = 3
	var action_points = $GameState/Info.players[my_internal_id].action_points[agent_selected]
	if abs(dx) == 1 or abs(dy) == 1:
		if pos in $GameState/TileMap.agents_pos:
			if action_points < constants.COUT_POUSSER:
				return
			animating = $GameState.push(agent_selected, dir, my_internal_id)
			if animating:
				socket.put_utf8_string("PUSH " + str(agent_selected) + " " + str(dir))
		else:
			if action_points < constants.COUT_DEPLACEMENT:
				return
			animating = $GameState.move(agent_selected, dir, my_internal_id)
			if animating:
				socket.put_utf8_string("MOVE " + str(agent_selected) + " " + str(dir))
	else:
		if action_points < constants.COUT_GLISSADE:
			return
		animating = $GameState.slide(agent_selected, dir, my_internal_id)
		if animating:
			socket.put_utf8_string("SLIDE " + str(agent_selected) + " " + str(dir))

func _input(event):
	if not my_turn:
		return
	if animating:
		return
	if event is InputEventMouseButton and event.button_index == BUTTON_RIGHT and event.pressed:
		var pos = $GameState/TileMap.world_to_map(event.position)
		if pos.x >= 0 and pos.y >= 0 and pos.x < $GameState/TileMap.walls.size() and pos.y < $GameState/TileMap.walls.size():
			_action(pos)
