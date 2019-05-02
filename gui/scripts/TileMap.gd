extends TileMap

var modulate_color = Color(1, 1, 1, 1)

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

func get_tile(x, y):
	return get_cell(x,y)

func set_ore_color(value):
	if value == 10:
		modulate_color = Color(0.5, 0.75, 1, 1)
	get_tileset().tile_set_modulate(3, modulate_color)

func init(blocks, ores, ropes, spawn1, spawn2):
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			var is_ore = false
			var is_rope = false
			var ore_o = null
			
			for rope in ropes:
				if rope.haut.y >= y or rope.bas.y <= y:
					is_rope = true
					break
				
			for ore in ores:
				if ore.pos == Vector2(x, y):
					is_ore = true
					ore_o = ore
					break
					
			if is_ore:
				set_ore_color(ore_o.value)
				set_cell(x, y, 3)
			elif Vector2(x, y) == spawn1 or Vector2(x, y) == spawn2:
				set_cell(x, y, 5)
			elif is_rope:
				set_cell(x, y, 4)
			else:
				set_cell(x, y, blocks[y][x])