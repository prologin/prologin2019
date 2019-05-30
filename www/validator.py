#!/usr/bin/env python3

import sys

MAP_SIZE = 31


def inside_map(row, col):
    return 0 <= row < MAP_SIZE and 0 <= col < MAP_SIZE


# Read the grid
grid = [['.' for _ in range(MAP_SIZE)] for __ in range(MAP_SIZE)]

for row in range(MAP_SIZE):
    line = input()

    if len(line) != MAP_SIZE:
        sys.exit('La grille n\'est pas de la bonne largeur: {}'
                 .format(len(line)))

    for col, char in enumerate(line):
        grid[row][col] = char

        if char not in ['.', 'X', '#']:
            sys.exit('Type de case invalide: `{}`'.format(char))

# Read spawns

spawn1 = tuple(map(int, input().split(' ')))
spawn2 = tuple(map(int, input().split(' ')))

if not inside_map(*spawn1):
    sys.exit('Le premier spawn est hors de la grille: {}'.format(spawn1))
if not inside_map(*spawn2):
    sys.exit('Le deuxième spawn est hors de la grille: {}'.format(spawn2))

if spawn1 == spawn2:
    sys.exit('Les spawns doivent être à des positions différentes')

if spawn1[1] == spawn2[1] and abs(spawn1[0] - spawn2[0]) == 1:
    sys.exit('Les spawns ne peuvent pas être superposés')

# Read minerals

nb_minerals = int(input())

for _ in range(nb_minerals):
    row, col, res, val = map(int, input().split(' '))

    if not inside_map(row, col):
        sys.exit('Minerai hors de la grille: {}'.format((row, col)))

    if grid[row][col] != 'X':
        sys.exit('Minerai non placé sur du granite: {}'.format((row, col)))

# Read ropes

nb_ropes = int(input())

for _ in range(nb_ropes):
    row, col = map(int, input().split(' '))

    if not inside_map(row, col):
        sys.exit('Corde hors de la map: {}'.format((row, col)))

    if grid[row][col] != '.':
        sys.exit('Corde non placée sur une case vide: {}')
