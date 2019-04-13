# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends Node

const DIR = [Vector2(-1, 0), Vector2(0, 1), Vector2(1, 0), Vector2(0, -1)]
const DIR_DIC = {'OUEST': 0, 'SUD': 1, 'EST': 2, 'NORD': 3}

var selected_tile = null
var selected_dwarf = -1
var _turn = 0
var _undo = [] # player, dwarf, AP, MP, dwarf_moved, pos

func dwarf_id_to_internal(dwarf_id, player_id):
	return dwarf_id + constants.NB_DWARF * player_id

func internal_to_dwarf_id(internal):
	return internal % constants.NB_DWARF

func move(dwarf_id, direction, player_id):
	var internal = dwarf_id_to_internal(dwarf_id, player_id)
	var destination = $TileMap.dwarf_pos[internal] + DIR[direction]
	if not $TileMap.is_cell_free(destination):
		return false
	_undo.append([player_id, dwarf_id, constants.COUT_DEPLACEMENT, \
			internal, $TileMap.dwarf_pos[internal]])
	$TileMap.move_dwarf(internal, destination, false, false)
	$Info.players[player_id].move_points[dwarf_id] -= constants.COUT_DEPLACEMENT
	$Info.redraw()
	return true

func climb(dwarf_id, player_id):
	var destination = $TileMap.dwarf_pos[internal] + DIR['NORTH']
	if not $TileMap.is_cell_free(destination):
		return false
	_undo.append([player_id, dwarf_id, constants.COUT_ESCALADER, \
			internal, $TileMap.dwarf_pos[internal]])
	$Info.players[player_id].move_points[dwarf_id] -= constants.COUT_ESCALADER
	return true

func climb_the_rope(dwarf_id, player_id, dir):
	var destination = $TileMap.dwarf_pos[internal] + DIR['NORTH']
	if $TileMap.get_tile($TileMap.dwarf_pos[internal]) != rope:
		return false
	_undo.append([player_id, dwarf_id, constants.COUT_ESCALADER, \
			internal, $TileMap.dwarf_pos[internal]])
	$Info.players[player_id].move_points[dwarf_id] -= constants.COUT_ESCALADER
	return true

func drop(dwarf_id, player_id, dir=1):
	var destination = $TileMap.dwarf_pos[internal] + DIR['NORTH']
	if $TileMap.get_tile($TileMap.dwarf_pos[internal]) != rope:
		return false
	_undo.append([player_id, dwarf_id, constants.COUT_LACHER, \
			internal, $TileMap.dwarf_pos[internal]])
	$Info.players[player_id].move_points[dwarf_id] -= constants.COUT_LACHER
	return true

func mine(dwarf_id, player_id, dir):
	_undo.append([player_id, dwarf_id, constants.COUT_MINER, \
			internal, $TileMap.dwarf_pos[internal]])
	$Info.players[player_id].action_points[dwarf_id] -= constants.COUT_MINER
	$Info.redraw()
	return true

func pull(dwarf_id, player_id):
	_undo.append([player_id, dwarf_id, constants.COUT_TIRER, \
			internal, $TileMap.dwarf_pos[internal]])
	$Info.players[player_id].action_points[dwarf_id] -= constants.COUT_TIRER
	return true

func set_a_rope(dwarf_id, player_id):
	#add turn
	return true

func undo():
	if _undo:
		var data = _undo.pop_back()
		$Info.players[data[0]].action_points[data[1]] += data[2]
		$Info.redraw()
		$TileMap.teleport_dwarf(data[3], data[4])

func init(dirt, dwarfs):
	$TileMap.init(dirt, agents)
	$Info.position.y = $TileMap.walls.size() * $TileMap.cell_size.y * $TileMap.scale.y

func set_turn(turn_index):
	var type = turn_index % 3
	_turn = (turn_index - type) / 3
	_undo = []
	$TileMap.update_ores(_turn)
	$Info.set_turn(_turn, type)
	_update_tile_info()
	_update_dwarf_info()

func _update_tile_info():
	if selected_tile == null:
		$Info/Tile.text = ""
		$TileMap/Select.visible = false
		return
	$TileMap/Select.visible = true
	$TileMap/Select.rect_position = $TileMap.map_to_world(selected_tile)
	var ores = null
	if $TileMap.get_cellv(selected_tile) == $TileMap.get_tileset().find_tile_by_name("Ores"):
		for a in $TileMap.ores:
			if a.pos == selected_tile:
				if _turn >= a.first_turn and _turn < a.first_turn + a.duration \
						and a.capture < constants.NB_TOURS_CAPTURE:
					ores = a
					break
	$Info.set_tile(selected_tile, $TileMap.dirt[selected_tile.x][selected_tile.y], ores)

func _update_dwarf_info():
	$Info.set_dwarf(selected_dwarf)

func select_dwarf(dwarf):
	if dwarf == selected_dwarf:
		dwarf = -1
	if selected_dwarf != -1:
		$TileMap.dwarfs[selected_dwarf].unfocus()
	if dwarf != -1:
		$TileMap.dwarfs[dwarf].focus
		()
	selected_dwarf = dwarf

func _input(event):
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		var pos = $TileMap.world_to_map(event.position)
		if pos.x >= 0 and pos.y >= 0 and pos.x < $TileMap.dirt.size() and pos.y < $TileMap.dirt.size():
			var dwarf = $TileMap.dwarf_pos.find(pos)
			if dwarf != -1:
				select_dwarf(dwarf)
				_update_dwarf_info()
			if not Input.is_action_pressed("ui_shift") or dwarf == -1:
				if selected_tile == pos:
					selected_tile = null
				else:
					selected_tile = pos
				_update_tile_info()

func replay_action(action, player_id):
	if action['atype'] == 'ID_ACTION_DEPLACER':
		return move(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_ESCALADER':
		return climb(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_GRIMPER':
		return climb_the_rope(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_LACHER':
		return drop(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_MINER':
		return mine(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_POSER_CORDE':
		return set_a_rope(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_TIRER':
		return pull(int(action['id_dwarf']), DIR_DIC[action['dir']], player_id)
	elif action['atype'] == 'ID_ACTION_DEBUG_AFFICHER_DRAPEAU':
		$TileMap.set_flag(player_id, Vector2(action['pos']['c'], action['pos']['r']), action['drapeau'])
	else:
		print("Unknown action ", action['atype'])
	return false

func _no_flags():
	$TileMap.flagNodes[0].set_visible(false)
	$TileMap.flagNodes[1].set_visible(false)

func _flags_j1():
	$TileMap.flagNodes[0].set_visible(true)
	$TileMap.flagNodes[1].set_visible(false)

func _flags_j2():
	$TileMap.flagNodes[0].set_visible(false)
	$TileMap.flagNodes[1].set_visible(true)

func _ready():
	$Info/FlagsNo.connect("pressed", self, "_no_flags")
	$Info/FlagsP1.connect("pressed", self, "_flags_j1")
	$Info/FlagsP2.connect("pressed", self, "_flags_j2")

func _process(delta):
	$Help.set_visible(Input.is_action_pressed("ui_h"))