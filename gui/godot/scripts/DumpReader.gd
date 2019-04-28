# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

static func parse_dump(filename):
	var file = File.new()
	file.open(filename, file.READ)
	var rounds = []
	
	while not file.eof_reached():
		rounds.append(JSON.parse(file.get_line()).result)
	
	return rounds

static func parse_dump_js():
	var text = JavaScript.eval('dump_value', true)
	var rounds = []
	for line in text.split('\n', false):
		rounds.append(JSON.parse(line).result)
	return rounds

class PlayerStats:
	var name = ""
	var score = 0
	var dwarfs = []
	#var history = []

class Ores:
	var pos = Vector2()
	var value = 0
	var duration = 0

class Map:
	var blocks = []
	var dwarfs = []

class Turn:
	var roundNumber = 0
	var roundTotal = 0
	var players = []
	var map_size = 0
	var ores = []
	var blocks = []

class CastIntSorter:
	static func sort(a, b):
		if int(a) < int(b):
			return true
		return false

static func parse_map(json, result):
	var cells = json["map"]["cells"]
	var size = sqrt(cells.size())
	result.map_size = size
	for c in range(size):
		result.blocks.append([])
		for r in range(size):
			result.block[c].append(cells[r * size + c] == "BLOCK")
	for ores_data in json["maps"]["ores"]:
		var ore = Ores.new()
		ore.pos = Vector2(ores_data["pos"]["c"], ores_data["pos"]["r"])
		ore.value = ores_data["something"]
		ore.duration = ores_data["something"]
		result.ores.append(ore)

static func parse_turn(json):
	var result = Turn.new()
	result.roundNumber = int(json["round"][0])
	result.roundTotal = int(json["round"][1])
	assert(json["players"].size() == 2)
	var player_ids = json["players"].keys().duplicate()
	player_ids.sort_custom(CastIntSorter, "sort")
	for player_id in player_ids:
		var node = json["players"][player_id]
		var player = PlayerStats.new()
		player.name = node["name"]
		player.score = node["score"]
		for i in range(node["agents"].size()):
			player.dwarfs.append(Vector2(-1, -1))
		for dwarf in node["agents"]:
			player.dwarfs[dwarf["id_nain"]] = Vector2(dwarf["pos"]["c"], dwarf["pos"]["r"])
		#player.history = node["history"]
		result.players.append(player)
	parse_map(json,result)
	return result
