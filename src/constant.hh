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
# define CONSTANT_HH_

#include <functional> // needed for std::hash

/// Taille de la mine (longueur et largeur).
# define TAILLE_MINE               31

/// Nombre de tours à jouer avant la fin de la partie.
# define NB_TOURS                  100

/// Nombre de points de déplacement par tour par nain (standard).
# define NB_POINTS_DEPLACEMENT     5

/// Nombre de points d'action par tour par nain (standard).
# define NB_POINTS_ACTION          6

/// Nombre de points de vie d'un nain (standard).
# define VIE_NAIN                  10

/// Nombre de nain (standard) par joueur.
# define NB_NAINS                  6

/// Nombre de points de déplacement pour qu'un nain (standard) se déplace d'une case.
# define COUT_DEPLACEMENT          1

/// Nombre de points de déplacement pour qu'un nain (standard) grimpe d'une case.
# define COUT_ESCALADER            1

/// Nombre de points d'action pour qu'un nain (standard) mine un bloc.
# define COUT_MINER                6

/// Nombre de points d'action pour qu'un nain (standard) tire sur une corde.
# define COUT_TIRER                1

/// Nombre de points d'action pour qu'un nain (standard) lâche une corde ou la paroi.
# define COUT_LACHER               0

/// Types de cases
typedef enum case_type {
  LIBRE, /* <- Case libre */
  GRANITE, /* <- Granite standard */
  OBSIDIENNE, /* <- Obsidienne */
  MINERAI, /* <- Minerai */
  ERREUR, /* <- Erreur */
} case_type;
// This is needed for old compilers
namespace std
{
  template <> struct hash<case_type> {
    size_t operator()(const case_type& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Direction
typedef enum direction {
  HAUT, /* <- Direction : haut */
  BAS, /* <- Direction : bas */
  GAUCHE, /* <- Direction : gauche */
  DROITE, /* <- Direction : droite */
} direction;
// This is needed for old compilers
namespace std
{
  template <> struct hash<direction> {
    size_t operator()(const direction& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Erreurs possibles
typedef enum erreur {
  OK, /* <- L'action s'est effectuée avec succès. */
  POSITION_INVALIDE, /* <- La position spécifiée n'est pas sur la mine. */
  DEPLACEMENT_HORS_LIMITES, /* <- Ce déplacement fait sortir un nain (standard) des limites de la mine. */
  DIRECTION_INVALIDE, /* <- La direction spécifiée n'existe pas. */
  DRAPEAU_INVALIDE, /* <- Le drapeau spécifié n'existe pas. */
} erreur;
// This is needed for old compilers
namespace std
{
  template <> struct hash<erreur> {
    size_t operator()(const erreur& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Types de drapeaux de débug
typedef enum debug_drapeau {
  AUCUN_DRAPEAU, /* <- Aucun drapeau, enlève le drapeau présent */
  DRAPEAU_BLEU, /* <- Drapeau bleu */
  DRAPEAU_VERT, /* <- Drapeau vert */
  DRAPEAU_ROUGE, /* <- Drapeau rouge */
} debug_drapeau;
// This is needed for old compilers
namespace std
{
  template <> struct hash<debug_drapeau> {
    size_t operator()(const debug_drapeau& v) const {
      return hash<int>()(static_cast<int>(v));
    }
  };
}

/// Position dans la mine, donnée par deux coordonnées.
typedef struct position {
  int ligne;  /* <- Coordonnée : ligne */
  int colonne;  /* <- Coordonnée : colonne */
} position;


/// Minerai à récolter
typedef struct minerai {
  int resistance;  /* <- Nombre de tours consécutifs de minage nécessaires */
  int rendement;  /* <- Valeur marchande du bloc de minerai */
} minerai;


/// Nain (standard)
typedef struct nain {
  position pos;  /* <- Position actuelle du nain (standard) */
  int vie;  /* <- Point(s) de vie restant du nain (standard) */
  int pa;  /* <- Point(s) d'action restant du nain (standard) */
  int pm;  /* <- Point(s) de déplacement restant du nain (standard) */
  int butin;  /* <- Valeur marchande total que le nain (standard) possède */
} nain;



#endif // !CONSTANT_HH_
