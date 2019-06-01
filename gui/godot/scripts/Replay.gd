extends Node

const DUMP_READER = preload("res://scripts/DumpReader.gd")

var dump = null
var turn = 0

var socket = null
var my_stechec_id = null
var waiting = false
var pause_init = false
var pause = false
var want_next_turn = true

var is_animating = false
var actions = []
var jump = false

var current_turn = null
var last_turn = null

func jump_turn(input_turn):
	turn = int(input_turn) * 2
	current_turn = DUMP_READER.parse_turn(dump[turn])
	$GameState.init(current_turn, self, true)
	is_animating = false
	$GameState.redraw(turn, current_turn.players, current_turn.ropes)
	get_tree().paused = false

func get_player_id():
	return 1 - (turn % Constants.NB_JOUEURS)

func finish_animating():
	is_animating = false

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
	if not Global.spectator:
		dump = DUMP_READER.parse_input_json()
		current_turn = DUMP_READER.parse_turn(dump[turn])
	else:
		$GameState/Info/Jump.set_visible(false)
		_init_socket()
		var available = 0
		while available == 0:
			available = socket.get_available_bytes()
		var dump_spect = socket.get_string(available)
		var json = JSON.parse(dump_spect).result
		if not json:
			print("invalid json ", dump_spect)
		current_turn = DUMP_READER.parse_turn(json)
		last_turn = current_turn
		socket.put_utf8_string("NEXT")
		waiting = true
	$GameState.init(current_turn, self)
	$GameState.check(current_turn)

func next_turn():
	if pause:
		$GameState/Info/endnode/isPause.text = "Jeu en pause press n pour le prochain tour"
		want_next_turn = true
		return
	want_next_turn = false
	pause = pause_init
	if turn == Constants.NB_TOURS * Constants.NB_JOUEURS:
		return
	turn += 1
	if Global.spectator:
		$GameState.ores = last_turn.ores
	else:
		$GameState.ores = current_turn.ores
		current_turn = DUMP_READER.parse_turn(dump[turn])
	actions = current_turn.players[get_player_id()].history.duplicate()
	$GameState.clear_flags()
	$GameState.redraw(turn, current_turn.players, current_turn.ropes)
	#print("--- ", turn, " ---")

func _unhandled_input(event):
	if Input.is_action_just_pressed("toggle_screen"):
    	OS.window_fullscreen = !OS.window_fullscreen

func _process(delta):
	if waiting:
		var available = socket.get_available_bytes()
		if available:
			var dump_spect = socket.get_string(available)
			var json = JSON.parse(dump_spect).result
			last_turn = current_turn
			current_turn = DUMP_READER.parse_turn(json)
			waiting = false
			next_turn()
		else:
			return

	$GameState/Info/Error.text = ""
	if Input.is_action_pressed("ui_escape") and not get_tree().paused:
		get_tree().paused = true
		$Pause.show()

	#if $GameState/Info/Jump.pressed:
	#	$GameState.finish_action()
	#	var input = $GameState/Info/Jump/TurnIndex.get_line(0)
	#	var reg = RegEx.new()
	#	reg.compile("[0-9]+")
	#	if reg.search(input) and int(input) <= 99 and int(input) >= 0:
	#		jump_turn(input)
	#	else:
	#		$GameState/Info/Error.text = "Invalid index"

	if Input.is_action_just_pressed("ui_select"):
		pause_init = not pause_init
		pause = pause_init
		if pause:
			$GameState/Info/endnode/isPause.text = "Pause (dès la fin du tour)"
		else:
			$GameState/Info/endnode/isPause.text = ""
		
	if Input.is_action_just_pressed("ui_right") and pause_init:
		pause = false
	#print("actions.size(): ", actions.size())

	while actions.size() != 0 and not is_animating:

		#var act = actions.front()
		#if act.action >= 0:
		#	if act.action == 0:
		#		print("deplacer ", act.dir)
		#	elif act.action == 3:
		#		print("miner ", act.dir)
		#	else:
		#		print(act)

		is_animating = $GameState.replay_action(actions.pop_front(), get_player_id())
	if not is_animating:
		if turn != 0 and not Global.spectator:
			$GameState.check(DUMP_READER.parse_turn(dump[turn]))
		if Global.spectator:
			if not pause:
				socket.put_utf8_string("NEXT")
				waiting = true
			else:
				$GameState/Info/endnode/isPause.text = "Jeu en pause press n pour le prochain tour"
		else:
			next_turn()