extends TileMap

func init(blocks):
	for y in range(Constants.TAILLE_MINE):
		for x in range(Constants.TAILLE_MINE):
			set_cell(x, y, blocks[y][x])