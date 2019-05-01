extends Node

onready var dwarf_scene = preload("res://scenes/Dwarf.tscn")

var dwarfs = []

func spawn_dwarf(player_id, pos):
	var dwarf = dwarf_scene.instance()
	dwarf.set_external_position(pos, $TileMap)
	dwarf.set_team(player_id)
	add_child(dwarf)
	return dwarf

func init(blocks, players):
	$TileMap.init(blocks)
	
	for player in range(Constants.NB_JOUEURS):
		dwarfs.append([])
		for dwarf_id in range(Constants.NB_NAINS):
			dwarfs[player].append(spawn_dwarf(player, players[player].dwarfs[dwarf_id].pos))