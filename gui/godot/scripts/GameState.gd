extends Node

onready var dwarf_g_scene = preload("res://scenes/Dwarf_green.tscn")
onready var dwarf_b_scene = preload("res://scenes/Dwarf_blue.tscn")
onready var flag_scene = preload("res://scenes/Flag.tscn")

#ONLY FOR SPECTATOR MODE#
var selected_dwarf = -1
var flags = []

func select_dwarf(dwarf):
	if dwarf == selected_dwarf:
		dwarf = -1
	if selected_dwarf != -1:
		$TileMap.dwarfs[selected_dwarf].unfocus()
	if dwarf != -1:
		$TileMap.dwarfs[dwarf].focus()
	selected_dwarf = dwarf

########################

var dwarfs = []
var ores = []

var is_roping = Vector2(-1, -1)

const DIRS = [ Vector2(0, -1), Vector2(0, 1), Vector2(-1, 0), Vector2(1, 0) ]
func get_position_offset(pos, dir):
	assert dir >= 0 and dir < len(DIRS)
	return pos + DIRS[dir]

func is_ore(pos):
	for ore in ores:
			if pos == ore.pos:
				return ore
	return null

func finish_action():
	if is_roping != Vector2(-1, -1):
		$TileMap.set_rope(is_roping)
		is_roping = Vector2(-1, -1)

func replay_action(action, player_id):
	finish_action()

	if action["action"] == Constants.ACTIONS.get("ACTION_DEPLACER"):
		return move(action, player_id)
	if action["action"] == Constants.ACTIONS.get("ACTION_MINER"):
		return mine(action, player_id)
	if action["action"] == Constants.ACTIONS.get("ACTION_POSER_CORDE"):
		return set_rope(action, player_id)
	if action["action"] == Constants.ACTIONS.get("ACTION_AGRIPPER"):
		return stick(action, player_id, true)
	if action["action"] == Constants.ACTIONS.get("ACTION_LACHER"):
		return stick(action, player_id, false)
	if action["action"] == Constants.ACTIONS.get("ACTION_TIRER"):
		return pull(action, player_id)
	if action["action"] == -1:
		return flag(action, player_id)
	if action["action"] == -2:
		return fall(action)
	if action["action"] == -3:
		return die(action, player_id)
	if action["action"] == -4:
		return extend_rope(action)
	if action["action"] == -5:
		return respawn_dwarf(action)
	if action["action"] == -6:
		return break_block(action)
	if action["action"] == -7:
		return pulled(action)
	print("unknown action: ", action["action"])
	return false

func pull(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	dwarf.pull()
	return true

func stick(action, player_id, is_stick):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	dwarf.set_sticky(is_stick)

func flag(action, player_id):
	var pos = Vector2(action["pos"]["c"], action["pos"]["l"])
	var ftype = action["drapeau"]
	var spawned_flag = flag_scene.instance()
	spawned_flag.position = $TileMap.world_position(pos)
	add_child(spawned_flag)
	spawned_flag.set_color(ftype)
	flags.append(spawned_flag)
	return false

func clear_flags():
	for flag in flags:
		remove_child(flag)
	flags.clear()

func check(current_turn):
	for player in range(Constants.NB_JOUEURS):
		for dwarf_id in range(Constants.NB_NAINS):
			var current_pos = dwarfs[player][dwarf_id].external_pos
			var theorical_pos = current_turn.players[player].dwarfs[dwarf_id].pos
			if current_pos != theorical_pos:
					dwarfs[player][dwarf_id].move_to(theorical_pos, $TileMap, 0, 0)
			#	dwarfs[player][dwarf_id].set_external_position(theorical_pos, $TileMap)
			#assert current_pos == theorical_pos
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			var a = current_turn.blocks[y][x]
			var b = $TileMap.get_tile(x, y)
			#assert (a == 0) == (b == 0 or b == 5)
			#if not ((a == 0) == (b == 0 or b == 5)):
			#	$TileMap.mine(Vector2(x, y))

func die(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	dwarf.die()
	return true

func set_rope(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = get_position_offset(dwarf.external_pos, int(action["dir"]))
	dwarf.set_rope_to()
	is_roping = dest
	return false

func move(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = get_position_offset(dwarf.external_pos, int(action["dir"]))
	dwarf.move_to(dest, $TileMap, $TileMap.get_tile(dwarf.external_pos.x, dwarf.external_pos.y), player_id)
	return true

func mine(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = get_position_offset(dwarf.external_pos, int(action["dir"]))
	dwarf.mine_to(dest, $TileMap)
	return true

func extend_rope(action):
	$TileMap.draw_rope(Vector2(action["pos"]["c"], action["pos"]["l"]))
	return false

func respawn_dwarf(action):
	var player_id = action["player_id"]
	var nain_id = action["id_nain"]
	var spawn_pos = action["spawn"]
	teleport(dwarfs[player_id][nain_id], Vector2(spawn_pos["c"], spawn_pos["l"]))
	dwarfs[player_id][nain_id].dying = false

func break_block(action):
	var pos = action["pos"]
	$TileMap.mine(Vector2(action["pos"]["c"], action["pos"]["l"]))

func fall(action):
	var player_id = int(action["player_id"])
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = Vector2(action["goal"]["c"], action["goal"]["l"])
	dwarf.move_to(dest, $TileMap, 0, player_id, true)
	return true

func grab(action, player_id):
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	dwarf.grab_to($TileMap)
	return true

func pulled(action):
	var player_id = int(action["player_id"])
	var dwarf_id = int(action["id_nain"])
	var dwarf = dwarfs[player_id][dwarf_id]
	var dest = Vector2(action["goal"]["c"], action["goal"]["l"])
	dwarf.move_to(dest, $TileMap, 0, player_id, true)
	dwarf.pull_to()

func spawn_dwarf(player_id, pos, parent_node):
	var dwarf = null
	if player_id == 0:
		dwarf = dwarf_b_scene.instance()
	else:
		dwarf = dwarf_g_scene.instance()
	dwarf.set_external_position(pos, $TileMap)
	dwarf.connect("finished_moving", parent_node, "finish_animating")
	add_child(dwarf)
	return dwarf

func redraw(turn, players, ropes):
	if (turn % Constants.NB_JOUEURS == 0):
		$Info/Turn.text = str(turn / Constants.NB_JOUEURS + 1) + " / 100"
	$"Info/Score 1".text = str(players[0].score)
	$"Info/Score 2".text = str(players[1].score)

	if turn / Constants.NB_JOUEURS + 1 == 100:
		if players[0].score == players[1].score:
			$Info/endnode/End.text = "Egalite"
			$Info/blue.show()
			$Info/green.show()
			$Info/blue.play("idle")
			$Info/green.play("idle")
			return
		if players[0].score > players[1].score:
			$Info/endnode/End.text = players[0].name + " est le plus riche!\n\n Appuyer sur Esc pour quitter"
			$Info/blue.show()
			$Info/green.show()
			$Info/blue.play("win")
			$Info/green.play("die")
		else:
			$Info/blue.show()
			$Info/green.show()
			$Info/blue.play("die")
			$Info/green.play("win")
			$Info/endnode/End.text = "Victoire de " + players[1].name + " est le plus riche!\n\n Appuyer sur Esc pour quitter"

func teleport(dwarf, pos):
	dwarf.set_external_position(pos, $TileMap)

func init(turn, parent_node, reinit=false):
	ores = turn.ores
	if (reinit):
		$TileMap.init(turn.blocks, turn.ores, turn.ropes, turn.players[0].dwarfs[0].pos, turn.players[1].dwarfs[0].pos, true)
	else:
		$TileMap.init(turn.blocks, turn.ores, turn.ropes, turn.players[0].dwarfs[0].pos, turn.players[1].dwarfs[0].pos)
		$Info.init(turn.players)

	for player in range(Constants.NB_JOUEURS):
		dwarfs.append([])
		for dwarf_id in range(Constants.NB_NAINS):
			if (reinit):
				teleport(dwarfs[player][dwarf_id], turn.players[player].dwarfs[dwarf_id].pos)
			else:
				dwarfs[player].append(spawn_dwarf(player, turn.players[player].dwarfs[dwarf_id].pos, parent_node))
