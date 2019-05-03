#!/usr/bin/env python3

# SPDX-License-Identifier: CC-BY-SA-3.0
# Copyright 2015 Arthur Vaïsse (https://stackoverflow.com/a/30045893)
# Copyright 2018 Paul Guénézan
# Copyright 2018 Thibault Allançon

from tkinter import Tk, Canvas, Frame, Button, StringVar, OptionMenu
from tkinter.filedialog import asksaveasfilename, askopenfilename
from PIL import Image, ImageTk
import tkinter.ttk as ttk

import sys


SIZE_GRID = 31
MAP_CELL_SIZE = 20

CELL_TYPES = {
    'libre': {'sprite': 'libre.png'},
    'granite': {'sprite': 'stone.png'},
    'rope': {'sprite': 'rope.png'},
    'minerai': {'sprite': 'mineral.png'},
    'obsidian': {'sprite': 'obsidian.png'},
    'spawn 1': {'color': 'red'},
    'spawn 2': {'color': 'blue'},
}
DEFAULT_CELL_TYPE = 'libre'

GRID_OFFSET = 50
TEXT_OFFSET = 30

SYMETRY = ["NONE", "CENT", "HORI", "VERT", "DIAG1", "DIAG2"]


# Load sprites for different types of cells
SPRITES = dict()

for cell_type, params in CELL_TYPES.items():
    if 'sprite' in params:
        SPRITES[cell_type] = Image.open(params['sprite']).resize(
            (MAP_CELL_SIZE, MAP_CELL_SIZE), Image.ANTIALIAS)
    else:
        color = params['color'] if 'color' in params else 'white'
        SPRITES[cell_type] = Image.new(
            'RGB', (MAP_CELL_SIZE, MAP_CELL_SIZE), color)


def get_opp(pos, sym):
    if sym == "CENT":
        return (SIZE_GRID - 1 - pos[0], SIZE_GRID - 1 - pos[1])
    if sym == "HORI":
        return (SIZE_GRID - 1 - pos[0], pos[1])
    if sym == "VERT":
        return (pos[0], SIZE_GRID - 1 - pos[1])
    if sym == "DIAG1":
        return (SIZE_GRID - 1 - pos[1], SIZE_GRID - 1 - pos[0])
    if sym == "DIAG2":
        return (pos[1], pos[0])
    return None


class Cell():
    def __init__(self, master, x, y):
        self.master = master
        self.x = x
        self.y = y

        self.cell_type = DEFAULT_CELL_TYPE
        self.img = None

    def set(self, _type):
        self.cell_type = _type
        self.draw()

    def draw(self):
        if self.master is not None:
            x = self.x * MAP_CELL_SIZE + GRID_OFFSET + MAP_CELL_SIZE // 2
            y = self.y * MAP_CELL_SIZE + GRID_OFFSET + MAP_CELL_SIZE // 2

            self.img = ImageTk.PhotoImage(SPRITES[self.cell_type])
            self.master.create_image((x, y), image=self.img)


class Grid(Canvas):
    def __init__(self, master):
        Canvas.__init__(self, master,
                        width=MAP_CELL_SIZE * SIZE_GRID + 2 * GRID_OFFSET,
                        height=MAP_CELL_SIZE * SIZE_GRID + 2 * GRID_OFFSET)

        self.menu = Frame(master)
        self.menu.pack(side="top")

        self.clean_button = Button(
            master, text="Clean", command=self.clean_grid)
        self.clean_button.pack(in_=self.menu, side="left")

        self.save_button = Button(master, text="Save", command=self.save_grid)
        self.save_button.pack(in_=self.menu, side="left")

        self.load_button = Button(master, text="Load", command=self.load_grid)
        self.load_button.pack(in_=self.menu, side="left")

        self.grid = [[Cell(self, x, y) for x in range(SIZE_GRID)]
                     for y in range(SIZE_GRID)]

        self.symetry = StringVar(master)
        self.symetry.set(SYMETRY[0])
        self.dd_symetry = OptionMenu(master, self.symetry, *SYMETRY)
        self.dd_symetry.pack(side="bottom")

        types = list(CELL_TYPES.keys())
        self.draw_type = StringVar(master)
        self.draw_type.set(DEFAULT_CELL_TYPE)
        self.dd_draw_type = OptionMenu(master, self.draw_type, *types)
        self.dd_draw_type.pack(side="bottom")

        self.bind("<Button-1>", self.event_set_cell)
        self.bind("<B1-Motion>", self.event_set_cell)

        self.draw()
        self.draw_coords()

    def draw(self):
        for row in self.grid:
            for cell in row:
                cell.draw()

    def draw_coords(self):
        for row in range(1, SIZE_GRID + 1):
            y_pos = MAP_CELL_SIZE * row - (MAP_CELL_SIZE // 2)
            self.create_text(TEXT_OFFSET, y_pos + GRID_OFFSET, text=str(row))

        for col in range(1, SIZE_GRID + 1):
            x_pos = MAP_CELL_SIZE * col - (MAP_CELL_SIZE // 2)
            self.create_text(x_pos + GRID_OFFSET, TEXT_OFFSET, text=str(col))

    #    ____     _ _
    #   / ___|___| | |___
    #  | |   / _ \ | / __|
    #  | |__|  __/ | \__ \
    #   \____\___|_|_|___/
    #

    @staticmethod
    def inside_grid(row, col):
        return 0 <= row < SIZE_GRID and 0 <= col < SIZE_GRID

    def set_cell(self, row, col):
        self.grid[row][col].set(self.draw_type.get())

        opp = get_opp((row, col), self.symetry.get())
        if opp is not None:
            self.grid[opp[0]][opp[1]].set(self.draw_type.get())

    def clean_grid(self):
        for row in self.grid:
            for cell in row:
                cell.set(DEFAULT_CELL_TYPE)

    #   _____                 _     _   _                 _ _ _
    #  | ____|_   _____ _ __ | |_  | | | | __ _ _ __   __| | (_)_ __   __ _
    #  |  _| \ \ / / _ \ '_ \| __| | |_| |/ _` | '_ \ / _` | | | '_ \ / _` |
    #  | |___ \ V /  __/ | | | |_  |  _  | (_| | | | | (_| | | | | | | (_| |
    #  |_____| \_/ \___|_| |_|\__| |_| |_|\__,_|_| |_|\__,_|_|_|_| |_|\__, |
    #                                                                 |___/

    def event_set_cell(self, event):
        row = int((event.y - GRID_OFFSET) / MAP_CELL_SIZE)
        col = int((event.x - GRID_OFFSET) / MAP_CELL_SIZE)

        if self.inside_grid(row, col):
            self.set_cell(row, col)

    #   ____            _       _ _          _   _
    #  / ___|  ___ _ __(_) __ _| (_)______ _| |_(_) ___  _ __
    #  \___ \ / _ \ '__| |/ _` | | |_  / _` | __| |/ _ \| '_ \
    #   ___) |  __/ |  | | (_| | | |/ / (_| | |_| | (_) | | | |
    #  |____/ \___|_|  |_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|
    #

    def save_grid(self):
        filename = asksaveasfilename()

        if not filename:
            return

        with open(filename, 'w+') as f:
            spawn1_pos = 0, 0
            spawn2_pos = 0, 1
            ropes = []
            minerals = []

            for x, row in enumerate(self.grid):
                for y, cell in enumerate(row):
                    if cell.cell_type in ['libre', 'rope', 'spawn 1', 'spawn 2']:
                        f.write('.')
                    elif cell.cell_type == 'obsidian':
                        f.write('#')
                    elif cell.cell_type in ['granite', 'minerai']:
                        f.write('X')
                    else:
                        f.write('?')

                    if cell.cell_type == 'rope':
                        ropes.append((x, y))
                    elif cell.cell_type == 'minerai':
                        minerals.append((x, y))
                    elif cell.cell_type == 'spawn 1':
                        spawn1_pos = x, y
                    elif cell.cell_type == 'spawn 2':
                        spawn2_pos = x, y

                f.write('\n')

            f.write('{} {}\n'.format(*spawn1_pos))
            f.write('{} {}\n'.format(*spawn2_pos))

            f.write('{}\n'.format(len(minerals)))
            for mineral_pos in minerals:
                f.write('{} {} 1 1\n'.format(*mineral_pos))

            f.write('{}\n'.format(len(ropes)))
            for rope_pos in ropes:
                f.write('{} {}\n'.format(*rope_pos))

    def load_grid(self, filename=None):
        if filename is None:
            filename = askopenfilename()

        with open(filename, 'r') as f:
            for row in range(SIZE_GRID):
                for col in range(SIZE_GRID):
                    c = f.read(1)

                    if c == '.':
                        self.grid[row][col].set('libre')
                    elif c == 'X':
                        self.grid[row][col].set('granite')
                    elif c == '#':
                        self.grid[row][col].set('obsidian')
                    else:
                        self.grid[row][col].set(DEFAULT_CELL_TYPE)

                f.read(1)

            y, x = map(int, f.readline().split(' '))
            self.grid[x][y].set('spawn 1')

            y, x = map(int, f.readline().split(' '))
            self.grid[x][y].set('spawn 2')

            nb_minerals = int(f.readline())
            for _ in range(nb_minerals):
                y, x, _, _ = map(int, f.readline().split(' '))
                self.grid[x][y].set('minerai')

            nb_ropes = int(f.readline())
            for _ in range(nb_ropes):
                y, x = map(int, f.readline().split(' '))
                self.grid[x][y].set('rope')

        self.draw()


if __name__ == "__main__":
    app = Tk()

    grid = Grid(app)
    if len(sys.argv) == 2:
        grid.load_grid(sys.argv[1])
    grid.pack()

    app.mainloop()
