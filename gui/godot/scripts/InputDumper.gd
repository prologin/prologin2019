# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends Node

const DumpReader = preload("res://scripts/DumpReader.gd")

var dump = null
var turn_index = 0
var actions_playing = []
var animating = false
var playing = false
var flags = []
var _tv_show = false

func _parse_json():
	if OS.has_feature('JavaScript'):
		return DumpReader.parse_dump_js()
	else:
		for arg in OS.get_cmdline_args():
			if arg.begins_with("-json="):
				var json = arg.right(6)
				print("Read dump ", json)
				return DumpReader.parse_dump(json)
	print("FATAL: could not retreive dump")
	get_tree().quit()

func _begin():
	#for arg in OS.get_cmdline_args():
	#	if arg == "-tv-show":
	#		_tv_show = true
	#		break
	#if not _tv_show:
	#	return
	OS.window_fullscreen = false
	$Names.set_visible(true)
	$Names/Champ1.text = $GameState/Info.players[0].name
	$Names/Champ2.text = $GameState/Info.players[1].name
	$Score1.set_visible(true)
	$Score2.set_visible(true)
	playing = true
	animating = true
	$GameState/Info/SpeedSlider.set_value(4)
	var timer = Timer.new()
	timer.connect("timeout", self, "_finish_animating")
	timer.set_wait_time(5)
	timer.start()
	add_child(timer)

func _end():
	#if _tv_show:
		$Names.set_visible(true)
		$Names/Champ1.text = str($GameState/Info.players[0].score)
		$Names/Champ2.text = str($GameState/Info.players[1].score)
		if $GameState/Info.players[0].score > $GameState/Info.players[1].score:
			$Names/VS.text = "Victoire : " + $GameState/Info.players[0].name
		elif $GameState/Info.players[0].score < $GameState/Info.players[1].score:
			$Names/VS.text = "Victoire : " + $GameState/Info.players[1].name
		else:
			$Names/VS.text = "Égalité"
		var timer = Timer.new()
		timer.connect("timeout", self, "_quit")
		timer.set_wait_time(5)
		timer.start()
		add_child(timer)

func _quit():
	get_tree().quit()

func _create_flags_maps():
	"""Go through all the rounds to create maps of debug flags"""
	flags.resize(Constants.NB_TOURS * 3)
	flags[0] = []
	for i in range(Constants.TAILLE_MINE * Constants.TAILLE_MINE * 2):
		flags[0].append(0)
	for index in range(1, flags.size()):
		flags[index] = flags[index - 1].duplicate()
		if index % 3:
			var i = (index - index % 3) / 3 * 2 + index % 3
			var state = DumpReader.parse_turn(dump[i])
			for player_id in range(2):
				for action in state.players[player_id].history:
					if action['action'] == Constants.ACTIONS.get('ID_ACTION_DEBUG_AFFICHER_DRAPEAU'):
						var byte = {'AUCUN_DRAPEAU': 0, 'DRAPEAU_ROUGE': 1, 'DRAPEAU_VERT': 2, 'DRAPEAU_BLEU': 3}[action['drapeau']]
						flags[index][(action['pos']['c'] * Constants.TAILLE_MINE + action['pos']['r']) * 2 + player_id] = byte

func _ready():
	dump = _parse_json()
	var init = DumpReader.parse_turn(dump[0])
	$GameState.init(init.blocks, init.players[0].dwarfs + init.players[1].dwarfs)
	for dwarf in $GameState/TileMap.dwarfs:
		 dwarf.connect("finished_moving", self, "_finish_animating")
	$GameState/Info.players[0].name = init.players[0].name
	$GameState/Info.players[1].name = init.players[1].name
	for ores_input in init.ores:
		var ore = $GameState/TileMap.Ores.new()
		ore.pos = ores_input.pos
		ore.value = ores_input.value
		ore.duration = ores_input.duration
		$GameState/TileMap.ores.append(ore)
	$GameState.set_turn(0)
	$GameState/Info.add_turn_slider().connect("value_changed", self, "_turn_slider")
	_create_flags_maps()
	_begin()

func _turn_slider(value):
	if int(value) != (turn_index - turn_index % 3) / 3:
		_jump(int(value) * 3)

func _finish_animating():
	animating = false
	$Names.set_visible(false)

func _dump_index():
	return (turn_index - turn_index % 3) / 3 * 2 + turn_index % 3

func _finish_last_turn(warn_teleport = true):
	"""The actions for a turn have been processed; now prepare next"""
	actions_playing = []
	var state = DumpReader.parse_turn(dump[_dump_index()])
	var size = state.players[0].dwarfs.size()
	for dwarf_id in range(size):
		for player_id in range(2):
			var pos = state.players[player_id].dwarfs[dwarf_id]
			if $GameState/TileMap.teleport_dwarf(dwarf_id + player_id * size, pos):
				if warn_teleport:
					print("Had to fix inconsistency in dump dwarf position")
					pass
	for x in range(Constants.TAILLE_MINE):
		for y in range(Constants.TAILLE_MINE):
			for player_id in range(2):
				$GameState/TileMap.set_flag(player_id, Vector2(x, y), \
						['AUCUN_DRAPEAU', 'DRAPEAU_ROUGE', 'DRAPEAU_VERT', 'DRAPEAU_BLEU'] \
						[flags[turn_index][(x * Constants.TAILLE_MINE + y) * 2 + player_id]])

func _update_ores():
	var state = DumpReader.parse_turn(dump[(turn_index)])
	for i in range(state.ores.size()):
		$GameState/TileMap.ores[i].duration = state.ores[i].duration
	$GameState/Info.players[0].score = state.players[0].score
	$GameState/Info.players[1].score = state.players[1].score
	#if _tv_show:
	$Score1.text = str(state.players[0].score)
	$Score2.text = str(state.players[1].score)

func _jump(index):
	turn_index = max(index - 1, 0)
	_finish_last_turn(false)
	turn_index = index
	_update_ores()
	$GameState.set_turn(turn_index)
	playing = false
	get_tree().paused = false

func _continue():
	_finish_last_turn()
	if turn_index + 1 == Constants.NB_TOURS * 3:
		_end()
		return
	turn_index += 1
	var state = DumpReader.parse_turn(dump[_dump_index()])
	if turn_index % 3:
		# We duplicate the array here in case we read it again
		actions_playing = state.players[turn_index % 3 - 1].history.duplicate()
	#else:
	_update_ores()
	$GameState.set_turn(turn_index)

func _process(delta):
	if Input.is_action_just_pressed("ui_home"):
		get_tree().quit()
	if Input.is_action_just_pressed("ui_select"):
		playing = !playing
		get_tree().paused = not playing
	if not animating:
		while actions_playing and not animating:
			animating = $GameState.replay_action(actions_playing.pop_front(), turn_index % 3 - 1)
	if not actions_playing and not animating and playing:
		_continue()
	if Input.is_action_just_pressed("ui_right") and turn_index < Constants.NB_TOURS * 3:
		_jump(turn_index + 1)
	elif Input.is_action_just_pressed("ui_left") and turn_index > 0:
		_jump(turn_index - 1)
