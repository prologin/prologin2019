extends Node

onready var dwarf_scene = preload("res://scenes/Dwarf.tscn")

var dwarfs = []

var is_mining = Vector2(-1, -1)
var is_roping = Vector2(-1, -1)

const DIRS = [ Vector2(0, -1), Vector2(0, 1), Vector2(-1, 0), Vector2(1, 0) ]
func get_position_offset(pos, dir):
	return pos + DIRS[dir]

func finish_action():
	if is_mining != Vector2(-1, -1):
		$TileMap.mine(is_mining)
		is_mining = Vector2(-1, -1)
		
	if is_roping != Vector2(-1, -1):
		$TileMap.rope(is_roping)
		is_roping = Vector2(-1, -1)

func replay_action(action, player_id):
	finish_action()

	if action["action"] == Constants.ACTIONS.get("ACTION_DEPLACER"):
		return move(action, player_id)
	if action["action"] == Constants.ACTIONS.get("ACTION_MINER"):
		return mine(action, player_id)
	if action["action"] == Constants.ACTIONS.get("ACTION_POSER_CORDE"):
		return set_rope(action, player_id)
	if action["action"] == -2:
		return fall(action)
	print("unknown action: ", action["action"])
	return false

func set_rope(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = get_position_offset(dwarf.external_pos, int(action["dir"]))
	dwarf.set_rope_to(dest, $TileMap)
	is_roping = dest
	return true

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

func fall(action):
	var player_id = int(action["player_id"])
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = Vector2(action["goal"]["c"], action["goal"]["l"])
	dwarf.move_to(dest, $TileMap)
	return true

func grab(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	dwarf.grab_to($TileMap)
	return true

func spawn_dwarf(player_id, pos, parent_node):
	var dwarf = dwarf_scene.instance()
	dwarf.set_external_position(pos, $TileMap)
	dwarf.set_team(player_id)
	dwarf.connect("finished_moving", parent_node, "finish_animating")
	add_child(dwarf)
	return dwarf

func redraw(turn, players, ropes):
	if (turn % 2 == 0):
		$Info/Turn.text = str(turn / 2 + 1) + " / 100"
	$"Info/Score 1".text = str(players[0].score)
	$"Info/Score 2".text = str(players[1].score)
	#$TileMap.update_ropes(ropes)

func init(turn, parent_node):
	$TileMap.init(turn.blocks, turn.ores, turn.ropes, turn.players[0].dwarfs[0].pos, turn.players[1].dwarfs[0].pos)
	$Info.init(turn.players)
	
	for player in range(Constants.NB_JOUEURS):
		dwarfs.append([])
		for dwarf_id in range(Constants.NB_NAINS):
			dwarfs[player].append(spawn_dwarf(player, turn.players[player].dwarfs[dwarf_id].pos, parent_node))