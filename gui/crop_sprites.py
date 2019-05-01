#!/usr/bin/python3
import os
import sys
import numpy
from PIL import Image


filename = sys.argv[1]
animations = [
    'move',
    'in_place',
    'mine',
    'rope',
    'hit',
    'die',
    'climb',
    'pull',]
tile_width = 30
tile_height = 30


img = Image.open(filename)
width, height = img.size

for y, name in enumerate(animations):
    for x in range(100):
        # Extract a non-empty area
        area = (x * tile_width, y * tile_height, (x + 1) * tile_width,
                (y + 1) * tile_height)
        cropped = img.crop(area)

        if not numpy.any(cropped):
            break

        # Save the file
        if not os.path.exists(name):
            os.makedirs(name)

        cropped.save(f'{name}/{x}.png')
