# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Sacha Delanoue

extends TileMap

var walls = []
var agents = []
var agents_pos = []
var flags = [[], []]
var flagNodes = [null, null]

class Alien:
	var pos = Vector2()
	var points = 0
	var first_turn = 0
	var duration = 0
	var capture = 0
var aliens = []

onready var character_scene = preload("res://scenes/Agent.tscn")

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

func spawn_agents():
	agents = []
	var nb_agents = agents_pos.size() / 2
	for i in range(2 * nb_agents):
		var character = character_scene.instance()
		character.position = world_position(agents_pos[i].x, agents_pos[i].y)
		character.set_team(i < nb_agents)
		add_child(character)
		agents.append(character)

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
	clear()
	var size = walls.size()
	flags = [[], []]
	for x in range(size):
		flags[0].append([])
		flags[1].append([])
		for y in range(size):
			set_cell(x, y, get_tile(x, y))
			flags[0][x].append(_new_flag(0, Vector2(x, y)))
			flags[1][x].append(_new_flag(1, Vector2(x, y)))

func update_aliens(turn):
	for alien in aliens:
		if turn >= alien.first_turn and turn < alien.first_turn + alien.duration \
				and alien.capture < constants.NB_TOURS_CAPTURE:
			set_cellv(alien.pos, get_tileset().find_tile_by_name("Alien"))
		else:
			set_cellv(alien.pos, get_tileset().find_tile_by_name("BG"))

func is_cell_free(pos):
	if pos.x < 0 or pos.y < 0 or pos.x >= walls.size() or pos.y >= walls.size():
		return false
	if walls[pos.x][pos.y] or agents_pos.has(pos):
		return false
	return true

func move_agent(i, dest, dash, pushed):
	agents[i].move_to(world_position(dest.x, dest.y), dash, pushed)
	agents_pos[i] = dest
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
