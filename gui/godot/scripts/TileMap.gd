# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends TileMap

var dwarfs = []
var dwarfs_pos = []
var flags = [[], []]
var flagNodes = [null, null]

#class Ores:
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
	var tile = "Wall" if walls[x][y] else "BG"
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
	var size = 32
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
		#fixme
	return true

func is_cell_free(pos):
	if pos.x < 0 or pos.y < 0 or pos.x >= constants.TAILLE_MINE or pos.y >= constants.TAILLE_MINE:
		return false
	if agents_pos.has(pos):
		return false
	return true

func move_dwarf(i, dest):
	dwarfs[i].move_to(world_position(dest.x, dest.y))
	dwarfs_pos[i] = dest
	return true

func teleport_agent(i, dest):
	agents[i].stop()
	if dest == agents_pos[i]:
		return false
	agents[i].position = world_position(dest.x, dest.y)
	agents_pos[i] = dest
	return true

func init(walls_grids, agents_positions):
	walls = walls_grids
	agents_pos = agents_positions
	set_map()
	spawn_agents()
