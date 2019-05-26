extends Node

const DumpReader = preload("res://scripts/DumpReader.gd")

var socket = null
var my_stechec_id = -1
var my_internal_id = -1
var interactive = false
var playing = false
var waiting = false
var animating = false
var myturn = false
var turn_index = 0
var current_turn = null
var dump = null
var actions_playing = []

func _init_socket():
	var port = 0;
	for arg in OS.get_cmdline_args():
		if arg.begins_with("-socket="):
			port = int(arg.right(8));
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
	dump = DumpReader.parse_input_json()
	var turn_init = DumpReader.parse_turn(json)
	$GameState.init(turn_init, self)
	for dwarf in $GameState/TileMap.dwarfs:
		dwarf.connect("finished_moving", self, "_finish_animating")
	$Info.init(turn_init.player)
	$TileMap.init(turn_init.blocks, turn_init.ores, turn_init.ropes, turn_init.players[0].dwarfs[0].pos, turn_init.players[1].dwarfs[0].pos)
	if interactive:
		$GameState.select_agent(my_internal_id * Constants.NB_NAINS)
		playing = true
		socket.put_utf8_string("NEXT")
		waiting = true

func _finish_animating():
	animating = false

func set_turn(input_turn):
	turn_index = int(input_turn) * 2
	current_turn = DumpReader.parse_turn(dump[turn_index])
	$GameState.init(current_turn, self, true)
	animating = false
	$GameState.redraw(turn_index, current_turn.players, current_turn.ropes)

var _futur_state = null

func _next_turn():
	if myturn:
		socket.put_utf8_string("NEXT")
		waiting = true
		myturn = false
		set_turn(turn_index + 1 + my_internal_id)
		return
	turn_index += 1
	set_turn(turn_index)
	myturn = false
	if turn_index % 3:
		if interactive:
			if turn_index % 3 - 1 == my_internal_id:
				myturn = true
			else:
				actions_playing = _futur_state.players[turn_index % 3 - 1].history
		else:
			socket.put_utf8_string("NEXT")
			waiting = true
	elif interactive:
		for i in range(_futur_state.ores.size()):
			$GameState/TileMap.ores[i].duration = _futur_state.ores[i].duration
			set_turn(turn_index)
			for player_id in range(2):
				$GameState/Info.players[player_id].score = _futur_state.players[player_id].score

func _process(delta):
	if waiting:
		var available = socket.get_available_bytes()
		if available and not interactive:
			var dump = socket.get_string(available)
			var json = JSON.parse(dump).result
			var state = DumpReader.parse_turn(json)
			for i in range(state.ores.size()):
				$GameState/TileMap.ores[i].duration = state.ores[i].duration
			set_turn(turn_index)
			actions_playing = state.players[turn_index % 3 - 1].history

func _action(pos):
	var dwarf_selected = $GameState.selected_dwarf
	if dwarf_selected < Constants.NB_NAINS * my_internal_id or dwarf_selected >= Constants.NB_NAINS * (1 + my_internal_id):
		return
	dwarf_selected -= Constants.NB_NAINS * my_internal_id
	var offset = my_internal_id * Constants.NB_NAINS
	var dwarf_pos = $GameState/TileMap.dwarfs_pos[dwarf_selected + offset]
	var dx = pos.x - dwarf_pos.x
	var dy = pos.y - dwarf_pos.y
	
	if dx != 0 and dy != 0:
		return
	if dx == 0 and dy == 0:
		return
	var dir = 0
	if dy > 0:
		dir = 1
	elif dy > 0:
		dir = 2
	elif dy < 0:
		dir = 3
	
	var action_points = $GameState/Info.players[my_internal_id].actions_points[dwarf_selected]
	var moving_points = $GameState/Info.players[my_internal_id].moving_points[dwarf_selected]
	if pos in $GameState/TileMap.dwars_pos:
		if action_points <  Constants.COUT_MINER:
			return
		animating = $GameState.mine(actions_playing, turn_index % Constants.NB_NAINS)
		if animating:
			socket.put_utf8_string("MINE " + str(dwarf_selected) + " " + str(dir))
			
		animating = $GameState.grab(actions_playing, turn_index % Constants.NB_NAINS)
		if animating:
			socket.put_utf8_string("GRAB " + str(dwarf_selected) + " " + str(dir))
	
	if pos in $GameState/TileMap.dwars_pos:
		if action_points <  Constants.COUT_LACHER:
			return
		animating = $GameState.mine(actions_playing, turn_index % Constants.NB_NAINS)
		if animating:
			socket.put_utf8_string("DROP " + str(dwarf_selected) + " " + str(dir))
	
