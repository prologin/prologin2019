# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends TileMap

var block = []
var dwarfs = []
var dwarfs_pos = []
var flags = [[], []]
var flagNodes = [null, null]

class Ores:
	var pos = Vector2()
	var value = 0;
	var duration = 0;
var ores = []

onready var character_scene = preload("res://scenes/Dwarf.tscn")

func _ready():
	for i in range(2):
		flagNodes[i] = Node2D.new()
		add_child(flagNodes[i])
		flagNodes[i].set_visible(false)

func world_position(x, y):
	return map_to_world(Vector2(x, y)) + get_cell_size() / 2

func get_tile(x, y):
	var tile = "Dirt" if block[x][y] else "Free"
	return get_tileset().find_tile_by_name(tile)

func spawn_dwarfs():
	dwarfs = []
	var nb_dwarfs = dwarfs_pos.size() / 2
	for i in range(2 * nb_dwarfs):
		var character = character_scene.instance()
		character.position = world_position(dwarfs_pos[i].x, dwarfs_pos[i].y)
		character.set_team(i < nb_dwarfs)
		add_child(character)
		dwarfs.append(character)

func set_flag(player, pos, type):
	var flag = flags[player][pos.x][pos.y]
	flag.set_visible(type != 'AUCUN_DRAPEAU')
	if flag.is_visible():
		var color = Color(0, 0, 0, 1)
		if type == 'DRAPEAU_BLEU':
			color.b = 1
		elif type == 'DRAPEAU_ROUGE':
			color.r = 1
		elif type == 'DRAPEAU_VERT':
			color.g = 1
		flag.set_modulate(color)

func _new_flag(player_id, pos):
	var sprite = Sprite.new()
	sprite.set_texture(load("res://assets/flag.png"))
	sprite.set_centered(false)
	sprite.set_offset(map_to_world(pos))
	sprite.set_visible(false)
	flagNodes[player_id].add_child(sprite)
	return sprite

func set_map():
	#sky on the first 2 lines
	#dig rock under
	clear()
	var size = block.size()
	flags = [[], []]
	for x in range(size):
		flags[0].append([])
		flags[1].append([])
		for y in range(size):
			set_cell(x, y, get_tile(x, y))
			flags[0][x].append(_new_flag(0, Vector2(x, y)))
			flags[1][x].append(_new_flag(1, Vector2(x, y)))

func update_ores(turn):
	for ore in ores:
		if ores.capture < Constants.NB_TOURS_CAPTURE:
			set_cellv(ore.pos, get_tileset().find_tile_by_name("Ores"))
		else:
			set_cellv(ore.pos, get_tileset().find_tile_by_name("Free"))

func is_cell_free(pos):
	if pos.x < 0 or pos.y < 0 or pos.x >= Constants.TAILLE_MINE or pos.y >= Constants.TAILLE_MINE:
		return false
	if dwarfs_pos.has(pos):
		return false
	return true

func move_dwarf(i, dest):
	dwarfs[i].move_to(world_position(dest.x, dest.y))
	dwarfs_pos[i] = dest
	return true

func teleport_dwarf(i, dest):
	dwarfs[i].stop()
	if dest == dwarfs_pos[i]:
		return false
	dwarfs[i].position = world_position(dest.x, dest.y)
	dwarfs_pos[i] = dest
	return true

func init(block_grids, dwarfs_positions):
	block = block_grids
	dwarfs_pos = dwarfs_positions
	set_map()
	spawn_dwarfs()
