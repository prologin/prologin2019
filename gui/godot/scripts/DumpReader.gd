# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

static func parse_dump(filename):
	var file = File.new()
	file.open(filename, file.READ)
	var rounds = []
	print("open")
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
	var history = []

class Ores:
	var pos = Vector2()
	var value = 0
	var duration = 0

class Dwarf:
	var pos = Vector2()
	var vie = 0
	var pa = 0
	var pm = 0
	var stick = 0
	var butin = 0

class Turn:
	var roundNumber = 0
	var roundTotal = 0
	var players = []
	var map_size = 0
	var ores = []
	var blocks = []
	var ropes = []

class CastIntSorter:
	static func sort(a, b):
		if int(a) < int(b):
			return true
		return false

static func parse_turn(json):
	var result = Turn.new()
	result.roundNumber = int(json["tour"][0])
	result.roundTotal = 100 #int(json["tour"][1])
	assert(json["joueurs"].size() == 2)
	var player_ids = json["joueurs"].keys().duplicate()
	player_ids.sort_custom(CastIntSorter, "sort")
	for player_id in player_ids:
		var node = json["joueurs"][player_id]
		var player = PlayerStats.new()
		player.name = node["name"]
		player.score = node["score"]
		for i in range(node["nains"].size()):
			player.dwarfs.append(Vector2(-1, -1))
		for dwarf in node["nains"]:
			player.dwarfs[dwarf["id_nain"]] = Vector2(dwarf["pos"]["c"], dwarf["pos"]["l"])
			player.dwarfs[dwarf["id_nain"]].pm = dwarf["pm"]
			player.dwarfs[dwarf["id_nain"]].pa = dwarf["pa"]
			player.dwarfs[dwarf["id_nain"]].stick = dwarf["accroche"]
			player.dwarfs[dwarf["id_nain"]].vie = dwarf["vie"]
			player.dwarfs[dwarf["id_nain"]].butin = dwarf["butin"]
		player.history = node["history"]
		result.players.append(player)
	var cells = json["map"]["cells"]
	var size = sqrt(cells.size())
	result.map_size = size
	for c in range(size):
		result.blocks.append([])
		for r in range(size):
			result.blocks[c].append(cells[r * size + c] == "BLOCK")
	for ores_data in json["maps"]["ores"]:
		var ore = Ores.new()
		ore.pos = Vector2(ores_data["pos"]["c"], ores_data["pos"]["l"])
		ore.value = ores_data["rendement"]
		ore.duration = ores_data["resistance"]
		result.ores.append(ore)
	return result
