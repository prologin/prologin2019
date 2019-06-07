/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_python.rb

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>
# include <vector>
# include <string>

/// Types de cases
typedef enum case_type {
  LIBRE, /* <- Case libre, qui peut abriter une corde et des nains (standard) */
  GRANITE, /* <- Granite (standard), qui peut cacher du minerai */
  OBSIDIENNE, /* <- Obsidienne */
  ERREUR_CASE, /* <- Erreur */
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
  ERREUR_DIRECTION, /* <- Erreur */
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
  PA_INSUFFISANTS, /* <- Votre nain (standard) ne possède pas assez de points d'action pour réaliser cette action. */
  PM_INSUFFISANTS, /* <- Votre nain (standard) ne possède pas assez de points de déplacement pour réaliser ce déplacement. */
  HORS_LIMITES, /* <- L'action est en dehors des limites de la mine. */
  DIRECTION_INVALIDE, /* <- La direction spécifiée n'existe pas, ou vous n'êtes pas autorisé à cibler cette direction pour cette action. */
  ID_NAIN_INVALIDE, /* <- Le nain (standard) spécifié n'existe pas. */
  OBSTACLE_MUR, /* <- La position spécifiée est un mur. */
  OBSTACLE_NAIN, /* <- La position spécifiée est un nain (standard) adverse. */
  OBSTACLE_CORDE, /* <- Il y a déjà une corde dans la direction spécifiée. */
  PAS_DE_CIBLE, /* <- Il n'y a pas de nain (standard) ni de granite (standard) sur la position spécifiée. */
  NAIN_MORT, /* <- Le nain (standard) spécifié est mort. */
  PAS_ACCROCHE, /* <- Le nain (standard) n'est pas accroché. */
  DEJA_ACCROCHE, /* <- Le nain (standard) est déjà accroché. */
  PAS_DE_CORDE, /* <- Il n'y a pas de corde dans la direction spécifiée. */
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

/// Types d'actions
typedef enum action_type {
  ACTION_DEPLACER, /* <- Action ``deplacer`` */
  ACTION_LACHER, /* <- Action ``lacher`` */
  ACTION_MINER, /* <- Action ``miner`` */
  ACTION_POSER_CORDE, /* <- Action ``poser_corde`` */
  ACTION_TIRER, /* <- Action ``tirer`` */
  ACTION_AGRIPPER, /* <- Action ``agripper`` */
} action_type;
// This is needed for old compilers
namespace std
{
  template <> struct hash<action_type> {
    size_t operator()(const action_type& v) const {
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
  int resistance;  /* <- Nombre de coups de pioches encore nécessaires avant que le bloc de minerais ne casse */
  int rendement;  /* <- Valeur marchande du bloc de minerai */
} minerai;


/// Nain (standard)
typedef struct nain {
  position pos;  /* <- Position actuelle du nain (standard) */
  int vie;  /* <- Point(s) de vie restant du nain (standard) */
  int pa;  /* <- Point(s) d'action restant du nain (standard) */
  int pm;  /* <- Point(s) de déplacement restant du nain (standard) */
  bool accroche;  /* <- Le nain (standard) est accroché à la paroi ou à une corde */
  int butin;  /* <- Valeur marchande totale que le nain (standard) possède */
} nain;


/// Action de déplacement représentée dans l'historique.
typedef struct action_hist {
  action_type atype;  /* <- Type de l'action */
  int id_nain;  /* <- Numéro du nain (standard) concerné par l'action */
  direction dir;  /* <- Direction visée par le nain (standard) durant le déplacement */
  direction sens;  /* <- Sens de l'action, utilisé uniquement pour préciser si l'on doit tirer une corde vers le bas ou vers le haut. Direction doit cibler la droite ou la gauche. */
} action_hist;



extern "C" {

/// Renvoie un chemin entre deux positions de la mine sous la forme d'une suite de directions à emprunter. Ce chemin minimise le nombre de blocs de granite nécessaire à casser. Si la position est invalide ou qu'il n'existe pas de tel chemin, le chemin renvoyé est vide.
std::vector<direction> api_chemin(position pos1, position pos2);

/// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
erreur api_deplacer(int id_nain, direction dir);

/// Le nain (standard) ``id_nain`` lâche la paroi.
erreur api_lacher(int id_nain);

/// Le nain (standard) ``id_nain`` s'agrippe à la paroi.
erreur api_agripper(int id_nain);

/// Le nain (standard) ``id_nain`` mine le bloc ou le nain (standard) dans la direction indiquée.
erreur api_miner(int id_nain, direction dir);

/// Le nain (standard) ``id_nain`` tire la corde dans le sens donné (HAUT ou BAS).
erreur api_tirer(int id_nain, direction dir_corde, direction sens);

/// Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.
erreur api_poser_corde(int id_nain, direction dir);

/// Affiche le drapeau spécifié sur la case indiquée.
erreur api_debug_afficher_drapeau(position pos, debug_drapeau drapeau);

/// Renvoie le type d'une case donnée.
case_type api_type_case(position pos);

/// Renvoie la liste de toutes les positions occupées par une corde dans la mine.
std::vector<position> api_liste_cordes();

/// Indique si une corde se trouve sur une case donnée.
bool api_corde_sur_case(position pos);

/// Renvoie le numéro du joueur à qui appartienent les nains (standard) sur la case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est invalide.
int api_nain_sur_case(position pos);

/// Renvoie la description du nain (standard) désigné par le numéro ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas présent sur la carte, tous les membres de la structure ``nain`` renvoyée sont initialisés à -1 (et le champ ``accroche`` à `false`).
nain api_info_nain(int id_joueur, int id_nain);

/// Renvoie la liste de tous les minerais dans la mine.
std::vector<position> api_liste_minerais();

/// Renvoie la description d'un minerai en fonction d'une position donnée. Si le minerai n'est pas présent sur la carte, ou si la position est invalide, tous les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
minerai api_info_minerai(position pos);

/// Renvoie le nombre de points de déplacement que coûterai le déplacement d'un nain (standard) dans une direction donnée. Renvoie -1 si le déplacement n'est pas possible.
int api_cout_de_deplacement(int id_nain, direction dir);

/// Renvoie la position de la taverne appartenant au joueur ``id_joueur``. Si le joueur n'existe pas, renvoie la position (-1, -1).
position api_position_taverne(int id_joueur);

/// Renvoie la liste des actions effectuées par l’adversaire durant son tour, dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans cette liste.
std::vector<action_hist> api_historique();

/// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est invalide.
int api_score(int id_joueur);

/// Renvoie votre numéro de joueur.
int api_moi();

/// Renvoie le numéro de joueur de votre adversaire.
int api_adversaire();

/// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à annuler ce tour ci.
bool api_annuler();

/// Retourne le numéro du tour actuel.
int api_tour_actuel();

/// Affiche le contenu d'une valeur de type case_type
void api_afficher_case_type(case_type v);

/// Affiche le contenu d'une valeur de type direction
void api_afficher_direction(direction v);

/// Affiche le contenu d'une valeur de type erreur
void api_afficher_erreur(erreur v);

/// Affiche le contenu d'une valeur de type action_type
void api_afficher_action_type(action_type v);

/// Affiche le contenu d'une valeur de type debug_drapeau
void api_afficher_debug_drapeau(debug_drapeau v);

/// Affiche le contenu d'une valeur de type position
void api_afficher_position(position v);

/// Affiche le contenu d'une valeur de type minerai
void api_afficher_minerai(minerai v);

/// Affiche le contenu d'une valeur de type nain
void api_afficher_nain(nain v);

/// Affiche le contenu d'une valeur de type action_hist
void api_afficher_action_hist(action_hist v);

}

#endif // !INTERFACE_HH_
