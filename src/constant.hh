/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2019 Prologin
*/

#ifndef CONSTANT_HH_
#define CONSTANT_HH_

#include <functional> // needed for std::hash

/// Taille de la mine (longueur et largeur).
#define TAILLE_MINE 31

/// Nombre de tours à jouer avant la fin de la partie.
#define NB_TOURS 100

/// Nombre de points de déplacement par tour par nain (standard).
#define NB_POINTS_DEPLACEMENT 5

/// Nombre de points d'action par tour par nain (standard).
#define NB_POINTS_ACTION 6

/// Nombre de points de vie d'un nain (standard).
#define VIE_NAIN 10

/// Nombre de joueurs.
#define NB_JOUEURS 2

/// Nombre de nains (standards) par joueur.
#define NB_NAINS 6

/// Dégât infligé par un coup de pioche à un nain (standard).
#define DEGAT_PIOCHE 3

/// Valeur cumulée maximale des minerais qu'un nain (standard) peut emporter
/// avec lui.
#define BUTIN_MAX 25

/// Nombre de points de déplacement pour qu'un nain (standard) se déplace d'une
/// case.
#define COUT_DEPLACEMENT 1

/// Nombre de points de déplacement pour qu'un nain (standard) grimpe d'une
/// case.
#define COUT_ESCALADER 2

/// Nombre de points de déplacement pour qu'un nain (standard) grimpe d'une case
/// lorsqu'il est agrippé à une corde.
#define COUT_ESCALADER_CORDE 1

/// Nombre de points d'action pour qu'un nain (standard) mine un bloc.
#define COUT_MINER 6

/// Nombre de points d'action pour qu'un nain (standard) tire sur une corde.
#define COUT_TIRER 1

/// Nombre de points d'action pour qu'un nain (standard) lâche la paroi.
#define COUT_LACHER 0

/// Nombre de points d'action pour qu'un nain (standard) s'agrippe à la paroi.
#define COUT_AGRIPPER 0

/// Types de cases
typedef enum case_type
{
    LIBRE,       /* <- Case libre, qui peut abriter une corde et des nains */
    GRANITE,     /* <- Granite (standard), qui peut cacher du minerai */
    OBSIDIENNE,  /* <- Obsidienne */
    ERREUR_CASE, /* <- Erreur */
} case_type;
// This is needed for old compilers
namespace std
{
template <> struct hash<case_type>
{
    size_t operator()(const case_type& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Direction
typedef enum direction
{
    HAUT,             /* <- Direction : haut */
    BAS,              /* <- Direction : bas */
    GAUCHE,           /* <- Direction : gauche */
    DROITE,           /* <- Direction : droite */
    ERREUR_DIRECTION, /* <- Erreur */
} direction;
// This is needed for old compilers
namespace std
{
template <> struct hash<direction>
{
    size_t operator()(const direction& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Erreurs possibles
typedef enum erreur
{
    OK,              /* <- L'action s'est effectuée avec succès. */
    PA_INSUFFISANTS, /* <- Votre nain (standard) ne possède pas assez de points
                        d'action pour réaliser cette action. */
    PM_INSUFFISANTS, /* <- Votre nain (standard) ne possède pas assez de points
                        de déplacement pour réaliser ce déplacement. */
    HORS_LIMITES,    /* <- L'action est en dehors des limites de la mine. */
    DIRECTION_INVALIDE, /* <- La direction spécifiée n'existe pas. */
    ID_NAIN_INVALIDE,   /* <- Le nain (standard) spécifié n'existe pas. */
    OBSTACLE_MUR,       /* <- La position spécifiée est un mur. */
    OBSTACLE_NAIN, /* <- La position spécifiée est un nain (standard) adverse.
                    */
    OBSTACLE_CORDE, /* <- Il y a déjà une corde dans la direction spécifiée. */
    PAS_DE_CIBLE,  /* <- Il n'y a pas de nain (standard) sur la position
                     spécifiée. */
    NAIN_MORT,     /* <- Le nain (standard) spécifié est mort. */
    PAS_ACCROCHE,  /* <- Le nain (standard) n'est pas accroché. */
    DEJA_ACCROCHE, /* <- Le nain (standard) est déjà accroché. */
    PAS_DE_CORDE,  /* <- Il n'y a pas de corde dans la direction spécifiée. */
    DRAPEAU_INVALIDE, /* <- Le drapeau spécifié n'existe pas. */
} erreur;
// This is needed for old compilers
namespace std
{
template <> struct hash<erreur>
{
    size_t operator()(const erreur& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Types d'actions
typedef enum action_type
{
    ACTION_DEPLACER,    /* <- Action ``deplacer`` */
    ACTION_LACHER,      /* <- Action ``lacher`` */
    ACTION_AGRIPPER,    /* <- Action ``agripper`` */
    ACTION_MINER,       /* <- Action ``miner`` */
    ACTION_POSER_CORDE, /* <- Action ``poser_corde`` */
    ACTION_TIRER,       /* <- Action ``tirer`` */
} action_type;
// This is needed for old compilers
namespace std
{
template <> struct hash<action_type>
{
    size_t operator()(const action_type& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Types de drapeaux de débug
typedef enum debug_drapeau
{
    AUCUN_DRAPEAU, /* <- Aucun drapeau, enlève le drapeau présent */
    DRAPEAU_BLEU,  /* <- Drapeau bleu */
    DRAPEAU_VERT,  /* <- Drapeau vert */
    DRAPEAU_ROUGE, /* <- Drapeau rouge */
} debug_drapeau;
// This is needed for old compilers
namespace std
{
template <> struct hash<debug_drapeau>
{
    size_t operator()(const debug_drapeau& v) const
    {
        return hash<int>()(static_cast<int>(v));
    }
};
} // namespace std

/// Position dans la mine, donnée par deux coordonnées.
typedef struct position
{
    int ligne;   /* <- Coordonnée : ligne */
    int colonne; /* <- Coordonnée : colonne */
} position;

/// Minerai à récolter
typedef struct minerai
{
    int resistance; /* <- Nombre de tours de minage nécessaires */
    int rendement;  /* <- Valeur marchande du bloc de minerai */
} minerai;

/// Nain (standard)
typedef struct nain
{
    position pos;  /* <- Position actuelle du nain (standard) */
    int vie;       /* <- Point(s) de vie restant du nain (standard) */
    int pa;        /* <- Point(s) d'action restant du nain (standard) */
    int pm;        /* <- Point(s) de déplacement restant du nain (standard) */
    bool accroche; /* <- Le nain (standard) est accroché à la paroi ou à la
                      corde */
    int butin; /* <- Valeur marchande total que le nain (standard) possède */
} nain;

/// Action de déplacement représentée dans l'historique.
typedef struct action_hist
{
    action_type atype; /* <- Type de l'action */
    int id_nain;    /* <- Numéro du nain (standard) concerné par l'action */
    direction dir;  /* <- Direction visée par le nain (standard) durant le
                       déplacement */
    direction sens; /* <- Sens de l'action, utilée par exemple pour préciser si
                       l'on doit tirer une corde vers le bas ou vers le haut. */
} action_hist;

#endif // !CONSTANT_HH_
