extends Node

const DUMP_READER = preload("res://scripts/DumpReader.gd")

var dump = null

func _ready():
	dump = DUMP_READER._parse_json()