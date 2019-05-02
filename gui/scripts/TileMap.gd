extends TileMap

func world_position(pos):
	return map_to_world(pos) + (get_cell_size() / 2)

func mine(pos):
	set_cell(pos.x, pos.y, 0)

func set_rope(pos):
	set_cell(pos.x, pos.y, 0)

func draw_rope(begin, end):
	var i = begin.y
	while i >= end.y:
		set_cell(begin.x,i,4)
		i += 1

func init(blocks, ores, ropes, spawn1, spawn2):
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			var is_ore = false
			var is_rope = false
			
			for rope in ropes:
				if rope.haut.y >= y or rope.bas.y <= y:
					is_rope = true
				
			for ore in ores:
				if ore.pos == Vector2(x, y):
					is_ore = true
					
			if is_ore:
				set_cell(x, y, 3)
			elif Vector2(x, y) == spawn1 or Vector2(x, y) == spawn2:
				set_cell(x, y, 5)
			elif is_rope:
				set_cell(x, y, 4)
			else:
				set_cell(x, y, blocks[y][x])