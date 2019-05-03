#!/usr/bin/env python3

# SPDX-License-Identifier: CC-BY-SA-3.0
# Copyright 2015 Arthur Vaïsse (https://stackoverflow.com/a/30045893)
# Copyright 2018 Paul Guénézan
# Copyright 2018 Thibault Allançon

from tkinter import Tk, Canvas, Frame, Button, StringVar, OptionMenu
from tkinter.filedialog import asksaveasfilename, askopenfilename
import tkinter.ttk as ttk

import sys

TAILLE_BANQUISE = 25

MAP_CELL_SIZE = 20

BORDER_COLOR = "black"
EMPTY_COLOR = "white"
FILLED_COLOR = "black"
GRID_OFFSET = 50
TEXT_OFFSET = 30

SYMETRY = ["NONE", "CENT", "HORI", "VERT", "DIAG1", "DIAG2"]


def get_opp(pos, sym):
    if (sym == "CENT"):
        return (TAILLE_BANQUISE - 1 - pos[0], TAILLE_BANQUISE - 1 - pos[1])
    if (sym == "HORI"):
      return (TAILLE_BANQUISE - 1 - pos[0], pos[1])
    if (sym == "VERT"):
      return (pos[0], TAILLE_BANQUISE - 1 - pos[1])
    if (sym == "DIAG1"):
      return (TAILLE_BANQUISE - 1 - pos[1], TAILLE_BANQUISE - 1 - pos[0])
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

    def draw(self):
        if self.master is not None:
            outline = BORDER_COLOR

            if self.fill:
                fill = FILLED_COLOR
            else:
                fill = EMPTY_COLOR

            xmin = self.x * MAP_CELL_SIZE + GRID_OFFSET
            xmax = xmin + MAP_CELL_SIZE
            ymin = self.y * MAP_CELL_SIZE + GRID_OFFSET
            ymax = ymin + MAP_CELL_SIZE

            self.master.create_rectangle(
                xmin, ymin, xmax, ymax, fill=fill, outline=outline)


class Grid(Canvas):
    def __init__(self, master):
        Canvas.__init__(self, master,
                        width=MAP_CELL_SIZE * TAILLE_BANQUISE+ 2 * GRID_OFFSET,
                        height=MAP_CELL_SIZE * TAILLE_BANQUISE + 2 * GRID_OFFSET)

        self.menu = Frame(master)
        self.menu.pack(side="top")

        self.clean_button = Button(master, text="Clean", command=self.clean_grid)
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

        self.bind("<Button-1>", self.handle_mouse_click)
        self.bind("<B1-Motion>", self.handle_mouse_motion)
        self.bind("<ButtonRelease-1>", lambda event: self.switched.clear())

        self.draw_cells()
        self.draw_coords()

    def draw_cells(self):
        for row in self.grid:
            for cell in row:
                cell.draw()

    def draw_coords(self):
        for row in range(1, TAILLE_BANQUISE + 1):
            y_pos = MAP_CELL_SIZE * row - (MAP_CELL_SIZE // 2)
            self.create_text(TEXT_OFFSET, y_pos + GRID_OFFSET, text=str(row))
        for col in range(1, TAILLE_BANQUISE + 1):
            x_pos = MAP_CELL_SIZE * col - (MAP_CELL_SIZE // 2)
            self.create_text(x_pos + GRID_OFFSET, TEXT_OFFSET, text=str(col))

    def get_coords(self, event):
        row = int((event.y - GRID_OFFSET) / MAP_CELL_SIZE)
        col = int((event.x - GRID_OFFSET) / MAP_CELL_SIZE)
        return row, col

    @staticmethod
    def inside_grid(event):
        return GRID_OFFSET <= event.x < GRID_OFFSET + TAILLE_BANQUISE * MAP_CELL_SIZE and \
               GRID_OFFSET <= event.y < GRID_OFFSET + TAILLE_BANQUISE * MAP_CELL_SIZE

    def handle_mouse_click(self, event):
        if not self.inside_grid(event):
            return
        row, col = self.get_coords(event)
        cell = self.grid[row][col]
        cell.switch()
        cell.draw()
        self.switched.append(cell)
        opp = get_opp((row, col), self.symetry.get())
        if opp != (-1, -1):
            cell = self.grid[opp[0]][opp[1]]
            cell.switch()
            cell.draw()
            self.switched.append(cell)

    def handle_mouse_motion(self, event):
        if not self.inside_grid(event):
            return
        row, col = self.get_coords(event)
        cell = self.grid[row][col]

        if cell not in self.switched:
            cell.switch()
            cell.draw()
            self.switched.append(cell)
            opp = get_opp((row, col), self.symetry.get())
            if opp != (-1, -1):
                cell = self.grid[opp[0]][opp[1]]
                cell.switch()
                cell.draw()
                self.switched.append(cell)

    def clean_grid(self):
        self.grid = []
        for row in range(TAILLE_BANQUISE):
            line = []
            for col in range(TAILLE_BANQUISE):
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

    def load_grid(self):
        filename = askopenfilename()
        if not filename:
            return
        self.load_grid(filename)

    def load_grid(self, filename):
        with open(filename, 'r') as f:
            for row in range(TAILLE_BANQUISE):
                for col in range(TAILLE_BANQUISE):
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
