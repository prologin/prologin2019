#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

import glob
import os.path

from wafgenerator import generator_player_install


def options(opt):
    pass


def configure(cfg):
    pass


def build(bld):
    bld.shlib(
        source='''
            src/action_deplacer.cc
            src/action_lacher.cc
            src/action_agripper.cc
            src/action_miner.cc
            src/action_tirer.cc
            src/action_poser_corde.cc
            src/action_debug_afficher_drapeau.cc
            src/api.cc
            src/dumper.cc
            src/entry.cc
            src/game_state.cc
            src/interface.cc
            src/map.cc
            src/player_info.cc
            src/rope.cc
            src/rules.cc
        ''',
        defines=['MODULE_COLOR=ANSI_COL_BROWN', 'MODULE_NAME="rules"'],
        target='prologin2019',
        use=['stechec2'],
    )

    abs_pattern = os.path.join(bld.path.abspath(), 'src/tests/test-*.cc')
    for test_src in glob.glob(abs_pattern):

        test_name = os.path.split(test_src)[-1]
        test_name = test_name[len("test-"):-(len(".cc"))]

        # Waf requires a relative path for the source
        src_relpath = os.path.relpath(test_src, bld.path.abspath())

        bld.program(
            features='gtest',
            source=src_relpath,
            target='prologin2019-test-{}'.format(test_name),
            use=['prologin2019', 'stechec2-utils'],
            includes=['.'],
            defines=['MODULE_COLOR=ANSI_COL_PURPLE',
                     'MODULE_NAME="prologin2019"'],
        )

    bld.install_files('${PREFIX}/share/stechec2/prologin2019', [
        'prologin2019.yml',
    ])

    generator_player_install(bld, 'prologin2019')
