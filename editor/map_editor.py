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
}
DEFAULT_TYPE = 'libre'

BORDER_COLOR = "black"
EMPTY_COLOR = "white"
FILLED_COLOR = "black"
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
    if (sym == "CENT"):
        return (SIZE_GRID - 1 - pos[0], SIZE_GRID - 1 - pos[1])
    if (sym == "HORI"):
        return (SIZE_GRID - 1 - pos[0], pos[1])
    if (sym == "VERT"):
        return (pos[0], SIZE_GRID - 1 - pos[1])
    if (sym == "DIAG1"):
        return (SIZE_GRID - 1 - pos[1], SIZE_GRID - 1 - pos[0])
    if (sym == "DIAG2"):
        return (pos[1], pos[0])
    return (-1, -1)


class Cell():
    def __init__(self, master, x, y):
        self.master = master
        self.x = x
        self.y = y
        self.fill = False

    def switch(self):
        self.fill = not self.fill

    def draw(self, cell_type):
        global img

        if self.master is not None:
            outline = BORDER_COLOR

            if self.fill:
                fill = FILLED_COLOR
            else:
                fill = EMPTY_COLOR

            xmin = self.x * MAP_CELL_SIZE + GRID_OFFSET + MAP_CELL_SIZE // 2
            ymin = self.y * MAP_CELL_SIZE + GRID_OFFSET + MAP_CELL_SIZE // 2

            self.img = ImageTk.PhotoImage(SPRITES[cell_type])
            self.master.create_image((xmin, ymin), image=self.img)


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

        self.clean_grid()
        self.switched = []

        self.symetry = StringVar(master)
        self.symetry.set(SYMETRY[0])
        self.dd_symetry = OptionMenu(master, self.symetry, *SYMETRY)
        self.dd_symetry.pack(side="bottom")

        types = list(CELL_TYPES.keys())
        self.draw_type = StringVar(master)
        self.draw_type.set(DEFAULT_TYPE)
        self.dd_draw_type = OptionMenu(master, self.draw_type, *types)
        self.dd_draw_type.pack(side="bottom")

        self.bind("<Button-1>", self.handle_mouse_click)
        self.bind("<B1-Motion>", self.handle_mouse_motion)
        self.bind("<ButtonRelease-1>", lambda event: self.switched.clear())

        self.draw_cells()
        self.draw_coords()

    def draw_cells(self):
        draw_type = DEFAULT_TYPE if not hasattr(
            self, 'draw_type') else self.draw_type.get()

        for row in self.grid:
            for cell in row:
                cell.draw(draw_type)

    def draw_coords(self):
        for row in range(1, SIZE_GRID + 1):
            y_pos = MAP_CELL_SIZE * row - (MAP_CELL_SIZE // 2)
            self.create_text(TEXT_OFFSET, y_pos + GRID_OFFSET, text=str(row))
        for col in range(1, SIZE_GRID + 1):
            x_pos = MAP_CELL_SIZE * col - (MAP_CELL_SIZE // 2)
            self.create_text(x_pos + GRID_OFFSET, TEXT_OFFSET, text=str(col))

    def get_coords(self, event):
        row = int((event.y - GRID_OFFSET) / MAP_CELL_SIZE)
        col = int((event.x - GRID_OFFSET) / MAP_CELL_SIZE)
        return row, col

    @staticmethod
    def inside_grid(event):
        return GRID_OFFSET <= event.x < GRID_OFFSET + SIZE_GRID * MAP_CELL_SIZE and \
            GRID_OFFSET <= event.y < GRID_OFFSET + SIZE_GRID * MAP_CELL_SIZE

    def handle_mouse_click(self, event):
        if not self.inside_grid(event):
            return

        row, col = self.get_coords(event)
        cell = self.grid[row][col]
        cell.switch()
        cell.draw(self.draw_type.get())
        self.switched.append(cell)
        opp = get_opp((row, col), self.symetry.get())

        if opp != (-1, -1):
            cell = self.grid[opp[0]][opp[1]]
            cell.switch()
            cell.draw(self.draw_type.get())
            self.switched.append(cell)

    def handle_mouse_motion(self, event):
        if not self.inside_grid(event):
            return

        row, col = self.get_coords(event)
        cell = self.grid[row][col]

        if cell not in self.switched:
            cell.switch()
            cell.draw(self.draw_type.get())
            self.switched.append(cell)
            opp = get_opp((row, col), self.symetry.get())

            if opp != (-1, -1):
                cell = self.grid[opp[0]][opp[1]]
                cell.switch()
                cell.draw(self.draw_type.get())
                self.switched.append(cell)

    def clean_grid(self):
        self.grid = []
        for row in range(SIZE_GRID):
            line = []
            for col in range(SIZE_GRID):
                line.append(Cell(self, col, row))
            self.grid.append(line)
        self.draw_cells()

    def save_grid(self):
        filename = asksaveasfilename()
        if not filename:
            return
        with open(filename, 'w+') as f:
            for row in self.grid:
                for cell in row:
                    if cell.fill:
                        f.write('X')
                    else:
                        f.write('.')
                f.write('\n')

    def load_grid(self, filename=None):
        if filename is None:
            filename = askopenfilename()

        with open(filename, 'r') as f:
            for row in range(SIZE_GRID):
                for col in range(SIZE_GRID):
                    c = f.read(1)
                    self.grid[row][col].fill = (c == 'X')
                f.read(1)

        self.draw_cells()


if __name__ == "__main__":
    app = Tk()

    grid = Grid(app)
    if len(sys.argv) == 2:
        grid.load_grid(sys.argv[1])
    grid.pack()

    app.mainloop()
