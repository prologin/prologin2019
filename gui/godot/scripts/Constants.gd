# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

# This file has been generated, you should not edit it

extends Node

const TAILLE_MINE = 31

const NB_TOURS = 100

const NB_POINTS_DEPLACEMENT = 5

const NB_POINTS_ACTION = 6

const VIE_NAIN = 10

const NB_JOUEURS = 2

const NB_NAINS = 6

const DEGAT_PIOCHE = 3

const COUT_DEPLACEMENT = 1

const COUT_ESCALADER = 2

const COUT_MINER = 6

const COUT_TIRER = 1

const COUT_LACHER = 0

const COUT_AGRIPPER = 0

const ACTIONS = {
	"ACTION_DEPLACER" : 0,
	"ACTION_LACHER" : 1,
	"ACTION_AGRIPPER" : 2,
	"ACTION_MINER" : 3,
	"ACTION_POSER_CORDE" : 4,
	"ACTION_TIRER" : 5,
	"ACTION_DEBUG_AFFICHER_DRAPEAU" : 6
}

enum flag {
	NONE,
	DRAPEAU_BLEU,
	DRAPEAU_VERT,
	DRAPEAU_ROUGE
}