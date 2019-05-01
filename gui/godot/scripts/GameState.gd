# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends Node

const DIR = [Vector2(0, 1), Vector2(0, -1), Vector2(-1, 0), Vector2(1, 0)]
const DIR_DIC = {'HAUT': 0, 'BAS': 1, 'GAUCHE': 2, 'DROITE': 3}

var selected_tile = null
var selected_dwarf = -1
var _turn = 0
var _undo = [] # player, dwarf, AP, MP, dwarf_moved, pos

func dwarf_id_to_internal(dwarf_id, player_id):
	return dwarf_id + Constants.NB_NAINS * player_id

func internal_to_dwarf_id(internal):
	return internal % Constants.NB_NAINS

func move(dwarf_id, direction, player_id):
	var internal = dwarf_id_to_internal(dwarf_id, player_id)
	var destination = $TileMap.dwarfs_pos[internal] + DIR[direction]
	if not $TileMap.is_cell_free(destination):
		return false
	#if DIR[direction] == DIR[1] or DIR[direction] == DIR[3]:
	#	if $TileMap.get_tile($TileMap.dwarfs_pos[internal].x, $TileMap.dwarfs_pos[internal].y) == 5.0:
	#		$Info.players[player_id].move_points[dwarf_id] -= Constants.COUT_ESCALADER_CORDE
	#	else:
	#		$Info.players[player_id].move_points[dwarf_id] -= Constants.COUT_ESCALADER
	#else:
	#	$Info.players[player_id].move_points[dwarf_id] -= Constants.COUT_DEPLACEMENT
	_undo.append([player_id, dwarf_id, Constants.COUT_DEPLACEMENT, \
			internal, $TileMap.dwarfs_pos[internal]])
	$TileMap.move_dwarf(internal, destination)
	$Info.redraw()
	return true
	
func drop(dwarf_id, player_id):
	var internal = dwarf_id_to_internal(dwarf_id, player_id)
	$TileMap.move_dwarf(internal, $TileMap.dwarfs_pos[internal] + DIR[1])
	$Info.redraw()
	return true

func mine(dwarf_id, player_id, dir):
	var internal = dwarf_id_to_internal(dwarf_id, player_id)
	var target = $TileMap.dwarfs_pos[internal] + DIR[dir]
	_undo.append([player_id, dwarf_id, Constants.COUT_MINER, \
			internal, $TileMap.dwarfs_pos[internal]])
	$Info.players[player_id].action_points[dwarf_id] -= Constants.COUT_MINER
	print("player id: ", player_id)
	$TileMap.mine_dwarf(internal, target)
	$Info.redraw()
	return true

func pull(dwarf_id, player_id):
	var internal = dwarf_id_to_internal(dwarf_id, player_id)
	_undo.append([player_id, dwarf_id, Constants.COUT_TIRER, \
			internal, $TileMap.dwarf_pos[internal]])
	$Info.players[player_id].action_points[dwarf_id] -= Constants.COUT_TIRER
	return true

func grab(dwarf_id, player_id):
	var internal = dwarf_id_to_internal(dwarf_id, player_id)
	_undo.append([player_id, dwarf_id, Constants.COUT_AGRIPPER, \
		internal, $TileMap.dwarfs_pos[internal]])
	#$Info.players[player_id]
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

func init(blocks, dwarfs):
	$TileMap.init(blocks, dwarfs)
	$Info.position.y = $TileMap.block.size() * $TileMap.cell_size.y * $TileMap.scale.y

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
				if a.duration == 0:
					ores = a
					break
	$Info.set_tile(selected_tile, $TileMap.block[selected_tile.x][selected_tile.y], ores)

func _update_dwarf_info():
	$Info.set_dwarf(selected_dwarf)

func select_dwarf(dwarf):
	if dwarf == selected_dwarf:
		dwarf = -1
	if selected_dwarf != -1:
		$TileMap.dwarfs[selected_dwarf].unfocus()
	if dwarf != -1:
		$TileMap.dwarfs[dwarf].focus()
	selected_dwarf = dwarf

func _input(event):
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		var pos = $TileMap.world_to_map(event.position)
		if pos.x >= 0 and pos.y >= 0 and pos.x < $TileMap.block.size() and pos.y < $TileMap.block.size():
			var dwarf = $TileMap.dwarfs_pos.find(pos)
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
	var direct = DIR_DIC.values()
	if action['action'] == Constants.ACTIONS.get("ACTION_DEPLACER"):
		return move(int(action['id_nain']), direct[action["dir"]], player_id)
	elif action['action'] == Constants.ACTIONS.get("ACTION_LACHER"):
		return drop(int(action['id_nain']), player_id)
	elif action['action'] == Constants.ACTIONS.get("ACTION_MINER"):
		return mine(int(action['id_nain']), action["dir"], player_id)
	elif action['action'] == Constants.ACTIONS.get("ACTION_POSER_CORDE"):
		return set_a_rope(int(action['id_nain']), player_id)
	elif action['action'] == Constants.ACTIONS.get("ACTION_TIRER"):
		return pull(int(action['id_nain']), player_id)
	elif action['action'] == Constants.ACTIONS.get("ACTION_AGRIPPER"):
		return grab(int(action['id_nain']), player_id)
	elif action['action'] == Constants.ACTIONS.get("ACTION_DEBUG_AFFICHER_DRAPEAU"):
		$TileMap.set_flag(player_id, Vector2(action['pos']['c'], action['pos']['l']), action['drapeau'])
	else:
		print("Unknown action ", action['action'])
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
