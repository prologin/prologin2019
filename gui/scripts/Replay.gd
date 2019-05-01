extends Node

const DUMP_READER = preload("res://scripts/DumpReader.gd")

var dump = null	
var turn = 0

func _ready():
	dump = DUMP_READER._parse_json()
	var current_turn = DUMP_READER.parse_turn(dump[turn])
	$GameState.init(current_turn.blocks, current_turn.players)