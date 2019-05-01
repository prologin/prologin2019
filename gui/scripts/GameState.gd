extends Node

onready var dwarf_scene = preload("res://scenes/Dwarf.tscn")

var dwarfs = []

var is_mining = Vector2(-1, -1)

const DIRS = [ Vector2(0, -1), Vector2(0, 1), Vector2(-1, 0), Vector2(1, 0) ]
func get_position_offset(pos, dir):
	return pos + DIRS[dir]

func finish_action():
	if is_mining != Vector2(-1, -1):
		$TileMap.mine(is_mining)
		is_mining = Vector2(-1, -1)

func replay_action(action, player_id):
	finish_action()

	if action["action"] == Constants.ACTIONS.get("ACTION_DEPLACER"):
		return move(action, player_id)
	if action["action"] == Constants.ACTIONS.get("ACTION_MINER"):
		return mine(action, player_id)
	print("unknown action: ", action["action"])
	return false

func move(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = get_position_offset(dwarf.external_pos, int(action["dir"]))
	dwarf.move_to(dest, $TileMap)
	return true

func mine(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = get_position_offset(dwarf.external_pos, int(action["dir"]))
	dwarf.mine_to(dest, $TileMap)
	is_mining = dest
	return true

func spawn_dwarf(player_id, pos, parent_node):
	var dwarf = dwarf_scene.instance()
	dwarf.set_external_position(pos, $TileMap)
	dwarf.set_team(player_id)
	dwarf.connect("finished_moving", parent_node, "finish_animating")
	add_child(dwarf)
	return dwarf

func init(blocks, players, parent_node):
	$TileMap.init(blocks)
	
	for player in range(Constants.NB_JOUEURS):
		dwarfs.append([])
		for dwarf_id in range(Constants.NB_NAINS):
			dwarfs[player].append(spawn_dwarf(player, players[player].dwarfs[dwarf_id].pos, parent_node))