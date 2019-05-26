#!/usr/bin/python3
#python crop_sprite.py all_blue.png blue
import os
import sys
import numpy
from PIL import Image


filename = sys.argv[1]
path = sys.argv[2]
timelines = [
    'move',
    'in_place',
    'mine',
    'rope',
    'hit',
    'die',
    'climb_vertical',
    'climb_horizontal',
    'fall',
    'pull_rope']
tile_width = 30
tile_height = 30


img = Image.open(filename)
width, height = img.size

if not os.path.exists(path):
            os.makedirs(path)

for y, frame_y in enumerate(timelines):
    for x in range(100):
        # Extract a non-empty area
        area = (x * tile_width, y * tile_height, (x + 1) * tile_width,
                (y + 1) * tile_height)
        cropped = img.crop(area)

        if numpy.sum(cropped) == 0:
            break

        # Save the file
        if not os.path.exists(path+'/'+timelines[y]):
            os.makedirs(path+'/'+timelines[y])

        cropped.save(f'{path}/{timelines[y]}/{x}.png')
