#!/usr/bin/env python3

# SPDX-License-Identifier: CC-BY-SA-3.0
# Copyright 2015 Arthur Vaïsse (https://stackoverflow.com/a/30045893)
# Copyright 2018 Paul Guénézan
# Copyright 2018 Thibault Allançon

import sys
from tkinter import Canvas, Frame, StringVar, Tk
from tkinter.filedialog import askopenfilename, asksaveasfilename
from tkinter.ttk import Button, OptionMenu, Style

import yaml
from PIL import Image, ImageTk

import os
res_dir = os.path.dirname(os.path.abspath(__file__))

# Display related constants

MAP_CELL_SIZE = 20
GRID_OFFSET = 20
TEXT_OFFSET = 5

SYMETRY = ["NONE", "CENT", "HORI", "VERT", "DIAG1", "DIAG2"]

# Load configuration
with open(res_dir + '/conf.yml') as f:
    CONF = yaml.load(f)

MAP_SIZE = CONF['map_size']
CELL_TYPES = CONF['cell_types']
DEFAULT_CELL_TYPE = CONF['default_cell_type']


def list_draw_types():
    for name, conf in CELL_TYPES.items():
        if 'extra' not in conf or 'default' in conf:
            yield name

        if 'presets' in conf:
            for preset_name in conf['presets']:
                yield f'{name}:{preset_name}'


def get_type_conf(name):
    if ':' not in name:
        return CELL_TYPES[name]

    name, preset = name.split(':')
    base_conf = CELL_TYPES[name].copy()
    preset_conf = CELL_TYPES[name]['presets'][preset]

    if 'sprite' in preset_conf or 'color' in preset_conf:
        base_conf.pop('sprite', None)
        base_conf.pop('color', None)

    base_conf.update(preset_conf)
    return base_conf


# Load sprites for different types of cells
SPRITES = dict()

for name in list_draw_types():
    conf = get_type_conf(name)

    if 'sprite' in conf:
        SPRITES[name] = Image.open(res_dir + '/' + conf['sprite']).resize(
            (MAP_CELL_SIZE, MAP_CELL_SIZE), Image.ANTIALIAS)
    else:
        color = conf['color'] if 'color' in conf else 'white'
        SPRITES[name] = Image.new(
            'RGB', (MAP_CELL_SIZE, MAP_CELL_SIZE), color)


def get_opp(pos, sym):
    if sym == "CENT":
        return (MAP_SIZE - 1 - pos[0], MAP_SIZE - 1 - pos[1])
    if sym == "HORI":
        return (MAP_SIZE - 1 - pos[0], pos[1])
    if sym == "VERT":
        return (pos[0], MAP_SIZE - 1 - pos[1])
    if sym == "DIAG1":
        return (MAP_SIZE - 1 - pos[1], MAP_SIZE - 1 - pos[0])
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
            x = self.x * MAP_CELL_SIZE + MAP_CELL_SIZE // 2
            y = self.y * MAP_CELL_SIZE + MAP_CELL_SIZE // 2

            self.img = ImageTk.PhotoImage(
                SPRITES[self.cell_type], master=self.master)
            self.master.create_image((x, y), image=self.img)


class Grid():
    def __init__(self, master):
        # Top menu
        self.menu = Frame(master)
        self.menu.pack(fill='both', expand=True)

        self.save_button = Button(master, text='Save',
                                  command=self.save_grid)
        self.save_button.pack(in_=self.menu, side='right')

        self.load_button = Button(master, text='Load',
                                  command=self.load_grid)
        self.load_button.pack(in_=self.menu, side='right')

        # Place the canvas
        self.canvas = Canvas(master,
                             width=MAP_CELL_SIZE * MAP_SIZE,
                             height=MAP_CELL_SIZE * MAP_SIZE)

        self.grid = [[Cell(self.canvas, row, col) for col in range(MAP_SIZE)]
                     for row in range(MAP_SIZE)]

        self.canvas.bind("<Button-1>", self.event_set_cell)
        self.canvas.bind("<B1-Motion>", self.event_set_cell)

        self.canvas.pack()

        # Tools
        self.tools = Frame(master)
        self.tools.pack(fill='both', expand=True)

        self.fill_button = Button(
            self.tools, text="Fill", command=self.fill_grid)
        self.fill_button.pack(side='left')

        types = list(list_draw_types())
        self.draw_type = StringVar(master)
        self.draw_type.set(DEFAULT_CELL_TYPE)
        self.dd_draw_type = OptionMenu(
            self.tools, self.draw_type, DEFAULT_CELL_TYPE, *types)
        self.dd_draw_type.pack(side='left')

        self.symetry = StringVar(master)
        self.symetry.set(SYMETRY[0])
        self.dd_symetry = OptionMenu(
            self.tools, self.symetry, SYMETRY[0], *SYMETRY)
        self.dd_symetry.pack(side='right')

        self.draw()

    def draw(self):
        for row in self.grid:
            for cell in row:
                cell.draw()

    #    ____     _ _
    #   / ___|___| | |___
    #  | |   / _ \ | / __|
    #  | |__|  __/ | \__ \
    #   \____\___|_|_|___/
    #

    @staticmethod
    def inside_grid(row, col):
        return 0 <= row < MAP_SIZE and 0 <= col < MAP_SIZE

    def set_cell(self, row, col):
        self.grid[row][col].set(self.draw_type.get())

        opp = get_opp((row, col), self.symetry.get())
        if opp is not None:
            self.grid[opp[0]][opp[1]].set(self.draw_type.get())

    def fill_grid(self):
        for row in self.grid:
            for cell in row:
                cell.set(self.draw_type.get())

    #   _____                 _     _   _                 _ _ _
    #  | ____|_   _____ _ __ | |_  | | | | __ _ _ __   __| | (_)_ __   __ _
    #  |  _| \ \ / / _ \ '_ \| __| | |_| |/ _` | '_ \ / _` | | | '_ \ / _` |
    #  | |___ \ V /  __/ | | | |_  |  _  | (_| | | | | (_| | | | | | | (_| |
    #  |_____| \_/ \___|_| |_|\__| |_| |_|\__,_|_| |_|\__,_|_|_|_| |_|\__, |
    #                                                                 |___/

    def event_set_cell(self, event):
        row = int(event.y / MAP_CELL_SIZE)
        col = int(event.x / MAP_CELL_SIZE)

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
                    conf = get_type_conf(cell.cell_type)
                    data = [row, col]

                    # Add extra infos if necessary
                    if 'extra' in conf:
                        data += [conf['default'][field]
                                 for field in conf['extra']]

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

        # Need to match exactly a preset when 'extra' is specified, create
        # an index to get which preset is used
        fullname_index = dict()

        for full_name in list_draw_types():
            if ':' in full_name:
                conf = get_type_conf(full_name)
                parent_name, preset_name = full_name.split(':')

                key = tuple([parent_name] + [conf['default'][field]
                                             for field in conf['extra']])
                fullname_index[key] = full_name
            else:
                fullname_index[(full_name, )] = full_name

        with open(filename, 'r') as f:
            # Read the grid
            for row in range(MAP_SIZE):
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

                        identifier = (name, ) + tuple(extra)
                        if identifier in fullname_index:
                            self.grid[row][col].set(fullname_index[identifier])

        self.draw()


if __name__ == "__main__":

    app = Tk()

    style = Style()
    style.theme_use('default')

    grid = Grid(app)

    if len(sys.argv) == 2:
        grid.load_grid(sys.argv[1])

    app.mainloop()
