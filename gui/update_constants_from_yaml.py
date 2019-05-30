#!/usr/bin/env python

import yaml

output=""
with open('../prologin2019.yml', 'r') as content:
    data = yaml.load(content)
    for constant in data['constant']:
        output += "\nconst {} = {}\n".format(constant['cst_name'],
                                            constant['cst_val'])

with open('godot/script/Constants.gd', 'w') as content:
    content.write("# SPDX-License-Identifier: GPL-3.0-or-later\n")
    content.write("# This file has been generated, you should not edit it\n\n")
    content.write("extends Node\n")
    content.write(output)
    content.write("\nconst ACTIONS = {\n"
                  "\"ACTION_DEPLACER\" : 0,\n"
                  "\"ACTION_LACHER\" : 1,\n"
                  "\"ACTION_AGRIPPER\" : 2,\n"
                  "\"ACTION_MINER\" : 3,\n"
                  "\"ACTION_POSER_CORDE\" : 4,\n"
                  "\"ACTION_TIRER\" : 5,\n"
                  "\"ACTION_DEBUG_AFFICHER_DRAPEAU\" : 6\n}\n\n")
    content.write("enum flag {\nNONE,\nDRAPEAU_BLEU,\nDRAPEAU_VERT,\nDRAPEAU_ROUGE\n}")
