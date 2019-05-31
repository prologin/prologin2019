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
    'pull_rope',
    'put_rope',
    'victory']
if sys.argv[3]:
    timelines = [timelines[int(sys.argv[3])]]
y_plus = [0]*len(timelines)
if sys.argv[3]:
    y_plus[0] += int(sys.argv[3])
tile_width = 30
tile_height = 30


img = Image.open(filename)
width, height = img.size

if not os.path.exists(path):
            os.makedirs(path)

for y_, frame_y in enumerate(timelines):
    y = y_ + y_plus[y_]
    for x in range(100):
        # Extract a non-empty area
        area = (x * tile_width, y * tile_height, (x + 1) * tile_width,
                (y + 1) * tile_height)
        cropped = img.crop(area)

        if numpy.sum(cropped) == 0:
            break

        # Save the file
        if not os.path.exists(path+'/'+timelines[y_]):
            os.makedirs(path+'/'+timelines[y_])

        cropped.save(f'{path}/{timelines[y_]}/{x}.png')
