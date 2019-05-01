# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

static func parse_dump(filename):
	var rounds = []
	var file = File.new()
	if file.open(filename, file.READ) != OK:
		printerr(file.open(filename, file.READ))
		return rounds

	var turn = 0
	while not file.eof_reached() && turn <= Constants.NB_TOURS * Constants.NB_JOUEURS:
		var json = JSON.parse(file.get_line())
		if json.error != OK:
			printerr("fail to parse round {r} : {e}".format({"r": turn, "e": json.error_string}))
			return rounds
		rounds.append(json.result)
		turn = turn + 1

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
	
class Rope:
	var haut = Vector2()
	var bas = Vector2()

class Turn:
	var roundNumber = 0
	var roundTotal = 0
	var players = []
	var ores = []
	var blocks = []
	var ropes = []

class CastIntSorter:
	static func sort(a, b):
		if int(a[0]) < int(b[0]):
			return true
		return false

static func parse_turn(json):
	var result = Turn.new()
	result.roundNumber = int(json["tour"][0])
	result.roundTotal = int(json["tour"][1])
	assert(json["joueurs"].size() == 2)
	var players = []
	for player in json["joueurs"]:
		players.append([player["id"], player])
	players.sort_custom(CastIntSorter, "sort")
	for p in players:
		var node = p[1]
		var player = PlayerStats.new()
		player.name = node["name"]
		player.score = node["score"]
		for i in range(node["nains"].size()):
			player.dwarfs.append(Dwarf.new())
		for dwarf in node["nains"]:
			player.dwarfs[dwarf["id_nain"]].pos  = Vector2(dwarf["pos"]["c"], dwarf["pos"]["l"])
			player.dwarfs[dwarf["id_nain"]].pm = dwarf["pm"]
			player.dwarfs[dwarf["id_nain"]].pa = dwarf["pa"]
			player.dwarfs[dwarf["id_nain"]].stick = dwarf["accroche"]
			player.dwarfs[dwarf["id_nain"]].vie = dwarf["vie"]
			player.dwarfs[dwarf["id_nain"]].butin = dwarf["butin"]
		player.history = node["historique"]
		result.players.append(player)
	var cells = json["carte"]["cases"]
	for c in range(Constants.TAILLE_MINE):
		result.blocks.append([])
		for r in range(Constants.TAILLE_MINE):
			result.blocks[c].append(cells[r * Constants.TAILLE_MINE + c])
	for ores_data in json["carte"]["minerais"]:
		var ore = Ores.new()
		ore.pos = Vector2(ores_data["pos"]["c"], ores_data["pos"]["l"])
		ore.value = ores_data["rendement"]
		ore.duration = ores_data["resistance"]
		result.ores.append(ore)
	for rope_data in json["carte"]["cordes"]:
		var rope = Rope.new()
		rope.haut = Vector2(rope_data["haut"]["c"], rope_data["haut"]["l"])
		rope.bas = Vector2(rope_data["bas"]["c"], rope_data["bas"]["l"])
	return result