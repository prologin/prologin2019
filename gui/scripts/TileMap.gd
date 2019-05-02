extends TileMap

func world_position(pos):
	return map_to_world(pos) + (get_cell_size() / 2)

func mine(pos):
	set_cell(pos.x, pos.y, 0)

func init(blocks, ores):
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			var is_ore = false
			for ore in ores:
				if ore.pos == Vector2(x, y):
					is_ore = true
			if is_ore:
				set_cell(x, y, 3)
			else:
				set_cell(x, y, blocks[y][x])