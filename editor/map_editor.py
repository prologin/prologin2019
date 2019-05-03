#!/usr/bin/env python3

# SPDX-License-Identifier: CC-BY-SA-3.0
# Copyright 2015 Arthur Vaïsse (https://stackoverflow.com/a/30045893)
# Copyright 2018 Paul Guénézan
# Copyright 2018 Thibault Allançon

import sys

from tkinter import Tk, Canvas, Frame, Button, StringVar, OptionMenu
from tkinter.filedialog import asksaveasfilename, askopenfilename
from PIL import Image, ImageTk


SIZE_GRID = 31
MAP_CELL_SIZE = 20

CELL_TYPES = {
    'libre': {
        'sprite': 'libre.png',
        'serialize': {
            'method': 'ascii',
            'ascii': '.',
        }
    },
    'granite': {
        'sprite': 'stone.png',
        'serialize': {
            'method': 'ascii',
            'ascii': 'X'
        }
    },
    'obsidian': {
        'sprite': 'obsidian.png',
        'serialize': {
            'method': 'ascii',
            'ascii': '#',
        }
    },
    'red_spawn': {
        'color': 'red',
        'serialize': {
            'method': 'raw',
            'order': 0,
        }
    },
    'blue_spawn': {
        'name': 'blue_spawn',
        'color': 'blue',
        'serialize': {
            'method': 'raw',
            'order': 1,
        }
    },
    'minerai': {
        'sprite': 'mineral.png',
        'extra': ['resistance', 'value'],
        'serialize': {
            'method': 'list',
            'ascii': 'X',
            'order': 2,
        }
    },
    'rope': {
        'sprite': 'rope.png',
        'serialize': {
            'method': 'list',
            'order': 3,
        }
    },
}

DEFAULT_CELL_TYPE = 'libre'

GRID_OFFSET = 50
TEXT_OFFSET = 30

SYMETRY = ["NONE", "CENT", "HORI", "VERT", "DIAG1", "DIAG2"]


# Load sprites for different types of cells
SPRITES = dict()

for name, conf in CELL_TYPES.items():
    if 'sprite' in conf:
        SPRITES[name] = Image.open(conf['sprite']).resize(
            (MAP_CELL_SIZE, MAP_CELL_SIZE), Image.ANTIALIAS)
    else:
        color = conf['color'] if 'color' in conf else 'white'
        SPRITES[name] = Image.new(
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
    def __init__(self, master, row, col):
        self.master = master
        self.x = col
        self.y = row

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

        self.grid = [[Cell(self, row, col) for col in range(SIZE_GRID)]
                     for row in range(SIZE_GRID)]

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
            # Init raw and list outputs datas
            raw_output = dict()
            list_output = dict()

            for name, conf in CELL_TYPES.items():
                if conf['serialize']['method'] == 'raw':
                    default_data = [0, 0] + ([0] * len(conf['extra'])
                                             if 'extra' in conf else [])
                    raw_output[conf['serialize']['order']] = default_data
                elif conf['serialize']['method'] == 'list':
                    list_output[conf['serialize']['order']] = []

            # Serialize the grid and catch raw and list outputs
            for row, row_data in enumerate(self.grid):
                for col, cell in enumerate(row_data):
                    conf = CELL_TYPES[cell.cell_type]
                    data = [row, col] + ([10] * len(conf['extra'])
                                         if 'extra' in conf else [])

                    # Serialization of the grid
                    if 'ascii' in conf['serialize']:
                        f.write(conf['serialize']['ascii'])
                    else:
                        f.write(CELL_TYPES[DEFAULT_CELL_TYPE]
                                ['serialize']['ascii'])

                    # Register for raw and list outputs
                    if conf['serialize']['method'] == 'raw':
                        raw_output[conf['serialize']['order']] = data
                    elif conf['serialize']['method'] == 'list':
                        list_output[conf['serialize']['order']].append(data)

                f.write('\n')

            # Serialize raw and list outputs
            order_keys = sorted(list(raw_output.keys()) +
                                list(list_output.keys()))

            for order in order_keys:
                if order in raw_output:
                    f.write('{}\n'.format(
                        ' '.join(map(str, raw_output[order]))))
                else:
                    f.write('{}\n'.format(len(list_output[order])))
                    for item in list_output[order]:
                        f.write('{}\n'.format(' '.join(map(str, item))))

    def load_grid(self, filename=None):
        if filename is None:
            filename = askopenfilename()

        # Index of symbols to consider in the grid
        from_ascii = {conf['serialize']['ascii']: name
                      for name, conf in CELL_TYPES.items()
                      if conf['serialize']['method'] == 'ascii'}

        # Ordered list of items to catch after reading the grid
        from_raw_or_list = [(name, conf['serialize']['order'])
                            for name, conf in CELL_TYPES.items()
                            if conf['serialize']['method'] in ['raw', 'list']]
        from_raw_or_list.sort(key=lambda x: x[1])
        from_raw_or_list = map(lambda x: x[0], from_raw_or_list)

        with open(filename, 'r') as f:
            # Read the grid
            for row in range(SIZE_GRID):
                for col, char in enumerate(f.readline()):
                    if self.inside_grid(row, col) and char in from_ascii:
                        self.grid[row][col].set(from_ascii[char])

            # Read raw and list infos
            for name in from_raw_or_list:
                conf = CELL_TYPES[name]

                if conf['serialize']['method'] == 'raw':
                    row, col, *extra = map(int, f.readline().split(' '))
                    self.grid[row][col].set(name)
                elif conf['serialize']['method'] == 'list':
                    n = int(f.readline())
                    for _ in range(n):
                        row, col, *extra = map(int, f.readline().split(' '))
                        self.grid[row][col].set(name)
        self.draw()


if __name__ == "__main__":
    app = Tk()

    grid = Grid(app)
    if len(sys.argv) == 2:
        grid.load_grid(sys.argv[1])
    grid.pack()

    app.mainloop()
