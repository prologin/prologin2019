# coding: iso-8859-1
from _api import *

# Taille de la mine (longueur et largeur).
TAILLE_MINE = 31

# Nombre de tours à jouer avant la fin de la partie.
NB_TOURS = 100

# Nombre de points de déplacement par tour par nain (standard).
NB_POINTS_DEPLACEMENT = 5

# Nombre de points d'action par tour par nain (standard).
NB_POINTS_ACTION = 6

# Nombre de points de vie d'un nain (standard).
VIE_NAIN = 10

# Nombre de joueurs.
NB_JOUEURS = 2

# Nombre de nains (standard) par joueur.
NB_NAINS = 6

# Dégât infligé par un coup de pioche à un nain (standard).
DEGAT_PIOCHE = 3

# Valeur cumulée maximale des minerais qu'un nain (standard) peut emporter avec lui.
BUTIN_MAX = 25

# Nombre de points de déplacement pour qu'un nain (standard) se déplace d'une case.
COUT_DEPLACEMENT = 1

# Nombre de points de déplacement pour qu'un nain (standard) se déplace d'une case lorsqu'il est agrippé.
COUT_ESCALADER = 2

# Nombre de points de déplacement pour qu'un nain (standard) se déplace vers une case occupée par une corde lorsqu'il est agrippé.
COUT_ESCALADER_CORDE = 1

# Nombre de points d'action pour qu'un nain (standard) mine un bloc.
COUT_MINER = 6

# Nombre de points d'action pour qu'un nain (standard) tire sur une corde.
COUT_TIRER = 1

# Nombre de points d'action pour qu'un nain (standard) lâche la paroi.
COUT_LACHER = 0

# Nombre de points d'action pour qu'un nain (standard) s'agrippe à la paroi.
COUT_AGRIPPER = 0


from enum import IntEnum

# Types de cases
class case_type(IntEnum):
    LIBRE = 0  # <- Case libre, qui peut abriter une corde et des nains (standard)
    GRANITE = 1  # <- Granite (standard), qui peut cacher du minerai
    OBSIDIENNE = 2  # <- Obsidienne
    ERREUR_CASE = 3  # <- Erreur


# Direction
class direction(IntEnum):
    HAUT = 0  # <- Direction : haut
    BAS = 1  # <- Direction : bas
    GAUCHE = 2  # <- Direction : gauche
    DROITE = 3  # <- Direction : droite
    ERREUR_DIRECTION = 4  # <- Erreur


# Erreurs possibles
class erreur(IntEnum):
    OK = 0  # <- L'action s'est effectuée avec succès.
    PA_INSUFFISANTS = 1  # <- Votre nain (standard) ne possède pas assez de points d'action pour réaliser cette action.
    PM_INSUFFISANTS = 2  # <- Votre nain (standard) ne possède pas assez de points de déplacement pour réaliser ce déplacement.
    HORS_LIMITES = 3  # <- L'action est en dehors des limites de la mine.
    DIRECTION_INVALIDE = 4  # <- La direction spécifiée n'existe pas, ou vous n'êtes pas autorisé à cibler cette direction pour cette action.
    ID_NAIN_INVALIDE = 5  # <- Le nain (standard) spécifié n'existe pas.
    OBSTACLE_MUR = 6  # <- La position spécifiée est un mur.
    OBSTACLE_NAIN = 7  # <- La position spécifiée est un nain (standard) adverse.
    OBSTACLE_CORDE = 8  # <- Il y a déjà une corde dans la direction spécifiée.
    PAS_DE_CIBLE = 9  # <- Il n'y a pas de nain (standard) ni de granite (standard) sur la position spécifiée.
    NAIN_MORT = 10  # <- Le nain (standard) spécifié est mort.
    PAS_ACCROCHE = 11  # <- Le nain (standard) n'est pas accroché.
    DEJA_ACCROCHE = 12  # <- Le nain (standard) est déjà accroché.
    PAS_DE_CORDE = 13  # <- Il n'y a pas de corde dans la direction spécifiée.
    DRAPEAU_INVALIDE = 14  # <- Le drapeau spécifié n'existe pas.


# Types d'actions
class action_type(IntEnum):
    ACTION_DEPLACER = 0  # <- Action ``deplacer``
    ACTION_LACHER = 1  # <- Action ``lacher``
    ACTION_MINER = 2  # <- Action ``miner``
    ACTION_POSER_CORDE = 3  # <- Action ``poser_corde``
    ACTION_TIRER = 4  # <- Action ``tirer``
    ACTION_AGRIPPER = 5  # <- Action ``agripper``


# Types de drapeaux de débug
class debug_drapeau(IntEnum):
    AUCUN_DRAPEAU = 0  # <- Aucun drapeau, enlève le drapeau présent
    DRAPEAU_BLEU = 1  # <- Drapeau bleu
    DRAPEAU_VERT = 2  # <- Drapeau vert
    DRAPEAU_ROUGE = 3  # <- Drapeau rouge


from collections import namedtuple

# Position dans la mine, donnée par deux coordonnées.

# Minerai à récolter
minerai = namedtuple("minerai",
    'resistance ' # <- Nombre de coups de pioches encore nécessaires avant que le bloc de minerais ne casse
    'rendement ' # <- Valeur marchande du bloc de minerai
)


# Nain (standard)
nain = namedtuple("nain",
    'pos ' # <- Position actuelle du nain (standard)
    'vie ' # <- Point(s) de vie restant du nain (standard)
    'pa ' # <- Point(s) d'action restant du nain (standard)
    'pm ' # <- Point(s) de déplacement restant du nain (standard)
    'accroche ' # <- Le nain (standard) est accroché à la paroi ou à une corde
    'butin ' # <- Valeur marchande totale que le nain (standard) possède
)


# Action de déplacement représentée dans l'historique.
action_hist = namedtuple("action_hist",
    'atype ' # <- Type de l'action
    'id_nain ' # <- Numéro du nain (standard) concerné par l'action
    'dir ' # <- Direction visée par le nain (standard) durant le déplacement
    'sens ' # <- Sens de l'action, utilisé uniquement pour préciser si l'on doit tirer une corde vers le bas ou vers le haut. Direction doit cibler la droite ou la gauche.
)


