# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Sacha Delanoue

class Map:
	var walls = []
	var agents = []

static func parse_map(filename):
	var file = File.new()
	file.open(filename ,file.READ)

	var map = Map.new()

	# Parse grid
	var map_size = file.get_line().length()
	file.seek(0)
	for i in range(map_size):
		map.walls.append([])
		var line = file.get_line()
		for j in range(map_size):
			map.walls[i].append(line[j] == 'X')

	# Parse agents stating position
	var nb_aliens = 0
	while true:
		var line = file.get_line().split(' ')
		if line.size() == 1:
			break
		assert(line.size() == 2)
		map.agents.append(Vector2(int(line[0]), int(line[1])))

	return map
