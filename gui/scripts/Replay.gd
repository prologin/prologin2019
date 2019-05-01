extends Node

const DUMP_READER = preload("res://scripts/DumpReader.gd")

var dump = null	
var turn = 0

var is_animating = false
var actions = []

func get_player_id():
	return turn % Constants.NB_JOUEURS

func finish_animating():
	is_animating = false

func _ready():
	dump = DUMP_READER.parse_input_json()
	var current_turn = DUMP_READER.parse_turn(dump[turn])
	$GameState.init(current_turn.blocks, current_turn.players, self)
	
func next_turn():
	print("next_turn()")
	if turn + 1 == Constants.NB_TOURS * 2:
		return
	turn += 1
	var current_turn = DUMP_READER.parse_turn(dump[turn])
	actions = current_turn.players[get_player_id()].history.duplicate()

func _process(delta):
	print("actions.size(): ", actions.size())
	while actions.size() != 0 and not is_animating:
		is_animating = $GameState.replay_action(actions.pop_front(), get_player_id())
	if not is_animating:
		next_turn()