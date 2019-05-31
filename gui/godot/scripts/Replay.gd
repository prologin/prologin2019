extends Node

const DUMP_READER = preload("res://scripts/DumpReader.gd")

var dump = null	
var turn = 0

var is_animating = false
var actions = []
var jump = false

var current_turn = null

func jump_turn(input_turn):
	turn = int(input_turn) * 2
	current_turn = DUMP_READER.parse_turn(dump[turn])
	$GameState.init(current_turn, self, true)
	is_animating = false
	$GameState.redraw(turn, current_turn.players, current_turn.ropes)
	get_tree().paused = false

func get_player_id():
	return turn % Constants.NB_JOUEURS

func finish_animating():
	is_animating = false

func _ready():
	dump = DUMP_READER.parse_input_json()
	current_turn = DUMP_READER.parse_turn(dump[turn])
	$GameState.init(current_turn, self)
	$GameState.check(current_turn)

func next_turn():
	if turn + 1 == Constants.NB_TOURS * Constants.NB_JOUEURS:
		return
	turn += 1
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
	$GameState/Info/Error.text = ""
	if Input.is_action_pressed("ui_escape") and not get_tree().paused:
		get_tree().paused = true
		$Pause.show()

	if $GameState/Info/Jump.pressed:
		$GameState.finish_action()
		var input = $GameState/Info/Jump/TurnIndex.get_line(0)
		var reg = RegEx.new()
		reg.compile("[0-9]+")
		if reg.search(input) and int(input) <= 99 and int(input) >= 0:
			jump_turn(input)
		else:
			$GameState/Info/Error.text = "Invalid index"

		
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
		if turn != 0:
			$GameState.check(DUMP_READER.parse_turn(dump[turn - 1]))
		next_turn()