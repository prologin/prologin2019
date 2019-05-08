#!/usr/bin/python3
import os
import sys
import numpy
from PIL import Image


filename = sys.argv[1]
timelines = [
    'move',
    'in_place',
    'mine',
    'rope',
    'hit',
    'die',
    'climb_vertical',
    'pull_rope']
tile_width = 30
tile_height = 30


img = Image.open(filename)
width, height = img.size

for y, frame_y in enumerate(timelines):
    for x in range(100):
        # Extract a non-empty area
        area = (x * tile_width, y * tile_height, (x + 1) * tile_width,
                (y + 1) * tile_height)
        cropped = img.crop(area)

        if numpy.sum(cropped) == 0:
            break

        # Save the file
        if not os.path.exists(timelines[y]):
            os.makedirs(timelines[y])

        cropped.save(f'{timelines[y]}/{x}.png')
