extends TileMap

func world_position(pos):
	return map_to_world(pos) + (get_cell_size() / 2);

func init(blocks):
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			set_cell(x, y, blocks[y][x])