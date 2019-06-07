# coding: utf-8

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Rémi Dupré

from api import *


DEPTH_FOR_ROPE = 4
MAX_CARRY = 20


def display_map():
    case_str = {case_type.LIBRE: '  ', case_type.GRANITE: '░░',
                case_type.OBSIDIENNE: '██', case_type.ERREUR_CASE: '??'}

    pos_nains = dict()
    pos_nains_adverse = dict()
    for id_nain in range(NB_NAINS):
        nain = info_nain(moi(), id_nain)
        pos_nains[nain.pos] = id_nain

        nain = info_nain(adversaire(), id_nain)
        pos_nains_adverse[nain.pos] = id_nain

    print('┏' + '━' * 2 * TAILLE_MINE + '┓')

    for x in range(TAILLE_MINE):
        print('┃', end='')

        for y in range(TAILLE_MINE):
            pos = x, y
            #  pos = position(colon(ne=x, ligne=y)
            disp = case_str[type_case(pos)]

            if corde_sur_case(pos):
                disp = disp[0] + '┊'

            if info_minerai(pos).resistance != -1:
                disp = '▚▚'

            if pos in pos_nains:
                disp = str(pos_nains[pos]) + disp[1]
            elif pos in pos_nains_adverse:
                disp = '⛏' + disp[1]

            print(disp, end='')

        print('┃')

    print('┗' + '━' * 2 * TAILLE_MINE + '┛')


def distance(pos1, pos2):
    return len(chemin(pos1, pos2))
    return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])


def dir_to_diff(dir):
    diffs = {direction.HAUT: (-1, 0), direction.BAS: (+1, 0),
             direction.GAUCHE: (0, -1), direction.DROITE: (0, +1), }
    return diffs[dir]


def follow_direction(pos, dir):
    diff = dir_to_diff(dir)
    return tuple(map(sum, zip(pos, diff)))


def depth(pos):
    ret = 0

    while (type_case(follow_direction(pos, direction.BAS))
            == case_type.LIBRE):
        ret += 1
        pos = follow_direction(pos, direction.BAS)

    return ret


def get_close_mineral(pos):
    ret = None
    dist_ret = float('inf')

    for minerai in liste_minerais():
        if distance(pos, minerai) < dist_ret:
            ret = minerai
            dist_ret = distance(pos, ret)

    return ret


def get_target(id_nain):
    nain = info_nain(moi(), id_nain)
    target = get_close_mineral(nain.pos)

    if target is None or nain.butin >= MAX_CARRY:
        target = position_taverne(moi())

    return target


def need_rope(path):
    if not path:
        return None

    count_downs = 0

    for dir in path[1:]:
        if dir == direction.BAS:
            count_downs += 1
        else:
            break

    return count_downs >= DEPTH_FOR_ROPE


def partie_init():
    pass


def jouer_tour():
    for id_nain in range(NB_NAINS):
        nain = info_nain(moi(), id_nain)
        target = get_target(id_nain)
        path = chemin(nain.pos, target)

        if need_rope(path):
            poser_corde(id_nain, path[0])
            debug_afficher_drapeau(follow_direction(
                nain.pos, path[0]), debug_drapeau.DRAPEAU_ROUGE)

    # Just move on retard path
    for id_nain in range(NB_NAINS):
        target = get_target(id_nain)
        debug_afficher_drapeau(target, debug_drapeau.DRAPEAU_VERT)

        for i in range(10):
            nain = info_nain(moi(), id_nain)
            debug_afficher_drapeau(nain.pos, target)
            path = chemin(nain.pos, target)

            if path:
                next_cell = follow_direction(nain.pos, path[0])

                if (type_case(next_cell) != case_type.LIBRE
                        or nain_sur_case(next_cell) == adversaire()):
                    miner(id_nain, path[0])

                if (need_rope(path) and not corde_sur_case(next_cell)
                        and path[0] in [direction.GAUCHE, direction.DROITE]):
                    debug_afficher_drapeau(follow_direction(
                        nain.pos, path[0]), debug_drapeau.DRAPEAU_ROUGE)
                    poser_corde(id_nain, path[0])
                    break

                if ((depth(next_cell) > 0 and path[0] == direction.HAUT)
                        or depth(next_cell) > 3):
                    agripper(id_nain)
                elif depth(nain.pos) == 0 or path[0] == direction.BAS:
                    lacher(id_nain)

                deplacer(id_nain, path[0])

    if moi() == 1:
        display_map()


def partie_fin():
    pass
