extends TileMap

var modulate_color = Color(1, 1, 1, 1)

#ONLY FOR SPECTATOR#

var dwarfs = []
var ores = []

###################

func world_position(pos):
	return map_to_world(pos) + (get_cell_size() / 2)

func set_flag(pos):
	set_cell(pos.x, pos.y, tile_set.find_tile_by_name("flag"))

func mine(pos):
	set_cell(pos.x, pos.y, tile_set.find_tile_by_name("Free"))

func set_rope(pos):
	set_cell(pos.x, pos.y, tile_set.find_tile_by_name("Rope"))

func draw_rope(begin):
	if get_tile(begin.x, begin.y) == 5:
		return
	set_cell(begin.x, begin.y, tile_set.find_tile_by_name("Rope_ext"))

func get_tile(x, y):
	return get_cell(x,y)

func get_ore_color(value):
	if value >= 1 and value <= 4:
		return tile_set.find_tile_by_name("Ores_black")
	if value >= 5 and value <= 9:
		return tile_set.find_tile_by_name("Ores_white")
	if value >= 10 and value <= 14:
		return tile_set.find_tile_by_name("Ores_yellow")
	if value >= 15 and value <= 19:
		return tile_set.find_tile_by_name("Ores_blue")
	if value >= 20:
		return tile_set.find_tile_by_name("Ores_green")
	return tile_set.find_tile_by_name("Ores_red")

func init(blocks, ores, ropes, spawn1, spawn2, reset=false):
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			var is_ore = false
			var is_rope_ext = false
			var is_rope = false
			var ore_o = null
			
			for rope in ropes:
				if rope.bas.x == x:
					if rope.haut.y == y:
						is_rope = true
					elif rope.haut.y < y and rope.bas.y >= y:
						is_rope_ext = true
						break
				
			for ore in ores:
				if ore.pos == Vector2(x, y):
					is_ore = true
					ore_o = ore
					break
					
			if is_ore:
				set_cell(x, y, get_ore_color(ore_o.value))
			elif not reset and (Vector2(x, y) == spawn1 or Vector2(x, y) == spawn2):
				set_cell(x, y, tile_set.find_tile_by_name("Spawn"))
			elif is_rope_ext:
				set_cell(x, y, tile_set.find_tile_by_name("Rope_ext"))
			elif is_rope:
				set_cell(x, y, tile_set.find_tile_by_name("Rope"))
			else:
				set_cell(x, y, blocks[y][x])