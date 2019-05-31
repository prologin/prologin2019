.. SPDX-License-Identifier: GPL-2.0-or-later
   Copyright 2019 Association Prologin <info@prologin.org>

===
API
===

..
   This file was generated using gen/make_sphinx.rsphinx
   Do not modify unless you are absolutely sure of what you are doing

Constantes
==========


.. c:var:: TAILLE_MINE

  :Valeur: 31
  :Description:
    Taille de la mine (longueur et largeur).

    

.. c:var:: NB_TOURS

  :Valeur: 100
  :Description:
    Nombre de tours à jouer avant la fin de la partie.

    

.. c:var:: NB_POINTS_DEPLACEMENT

  :Valeur: 5
  :Description:
    Nombre de points de déplacement par tour par nain (standard).

    

.. c:var:: NB_POINTS_ACTION

  :Valeur: 6
  :Description:
    Nombre de points d'action par tour par nain (standard).

    

.. c:var:: VIE_NAIN

  :Valeur: 10
  :Description:
    Nombre de points de vie d'un nain (standard).

    

.. c:var:: NB_JOUEURS

  :Valeur: 2
  :Description:
    Nombre de joueurs.

    

.. c:var:: NB_NAINS

  :Valeur: 6
  :Description:
    Nombre de nains (standard) par joueur.

    

.. c:var:: DEGAT_PIOCHE

  :Valeur: 3
  :Description:
    Dégât infligé par un coup de pioche à un nain (standard).

    

.. c:var:: BUTIN_MAX

  :Valeur: 25
  :Description:
    Valeur cumulée maximale des minerais qu'un nain (standard) peut emporter avec lui.

    

.. c:var:: COUT_DEPLACEMENT

  :Valeur: 1
  :Description:
    Nombre de points de déplacement pour qu'un nain (standard) se déplace d'une case.

    

.. c:var:: COUT_ESCALADER

  :Valeur: 2
  :Description:
    Nombre de points de déplacement pour qu'un nain (standard) se déplace d'une case lorsqu'il est agrippé.

    

.. c:var:: COUT_ESCALADER_CORDE

  :Valeur: 1
  :Description:
    Nombre de points de déplacement pour qu'un nain (standard) se déplace vers une case occupée par une corde lorsqu'il est agrippé.

    

.. c:var:: COUT_MINER

  :Valeur: 6
  :Description:
    Nombre de points d'action pour qu'un nain (standard) mine un bloc.

    

.. c:var:: COUT_TIRER

  :Valeur: 1
  :Description:
    Nombre de points d'action pour qu'un nain (standard) tire sur une corde.

    

.. c:var:: COUT_LACHER

  :Valeur: 0
  :Description:
    Nombre de points d'action pour qu'un nain (standard) lâche la paroi.

    

.. c:var:: COUT_AGRIPPER

  :Valeur: 0
  :Description:
    Nombre de points d'action pour qu'un nain (standard) s'agrippe à la paroi.

    


Énumérations
============



.. c:type:: case_type

  :Description:
      Types de cases
  :Valeurs:
    
      :LIBRE:
        Case libre, qui peut abriter une corde et des nains (standard)

    
      :GRANITE:
        Granite (standard), qui peut cacher du minerai

    
      :OBSIDIENNE:
        Obsidienne

    
      :ERREUR_CASE:
        Erreur

    


.. c:type:: direction

  :Description:
      Direction
  :Valeurs:
    
      :HAUT:
        Direction : haut

    
      :BAS:
        Direction : bas

    
      :GAUCHE:
        Direction : gauche

    
      :DROITE:
        Direction : droite

    
      :ERREUR_DIRECTION:
        Erreur

    


.. c:type:: erreur

  :Description:
      Erreurs possibles
  :Valeurs:
    
      :OK:
        L'action s'est effectuée avec succès.

    
      :PA_INSUFFISANTS:
        Votre nain (standard) ne possède pas assez de points d'action pour réaliser cette action.

    
      :PM_INSUFFISANTS:
        Votre nain (standard) ne possède pas assez de points de déplacement pour réaliser ce déplacement.

    
      :HORS_LIMITES:
        L'action est en dehors des limites de la mine.

    
      :DIRECTION_INVALIDE:
        La direction spécifiée n'existe pas, ou vous n'êtes pas autorisé à cibler cette direction pour cette action.

    
      :ID_NAIN_INVALIDE:
        Le nain (standard) spécifié n'existe pas.

    
      :OBSTACLE_MUR:
        La position spécifiée est un mur.

    
      :OBSTACLE_NAIN:
        La position spécifiée est un nain (standard) adverse.

    
      :OBSTACLE_CORDE:
        Il y a déjà une corde dans la direction spécifiée.

    
      :PAS_DE_CIBLE:
        Il n'y a pas de nain (standard) ni de granite (standard) sur la position spécifiée.

    
      :NAIN_MORT:
        Le nain (standard) spécifié est mort.

    
      :PAS_ACCROCHE:
        Le nain (standard) n'est pas accroché.

    
      :DEJA_ACCROCHE:
        Le nain (standard) est déjà accroché.

    
      :PAS_DE_CORDE:
        Il n'y a pas de corde dans la direction spécifiée.

    
      :DRAPEAU_INVALIDE:
        Le drapeau spécifié n'existe pas.

    


.. c:type:: action_type

  :Description:
      Types d'actions
  :Valeurs:
    
      :ACTION_DEPLACER:
        Action ``deplacer``

    
      :ACTION_LACHER:
        Action ``lacher``

    
      :ACTION_MINER:
        Action ``miner``

    
      :ACTION_POSER_CORDE:
        Action ``poser_corde``

    
      :ACTION_TIRER:
        Action ``tirer``

    
      :ACTION_AGRIPPER:
        Action ``agripper``

    


.. c:type:: debug_drapeau

  :Description:
      Types de drapeaux de débug
  :Valeurs:
    
      :AUCUN_DRAPEAU:
        Aucun drapeau, enlève le drapeau présent

    
      :DRAPEAU_BLEU:
        Drapeau bleu

    
      :DRAPEAU_VERT:
        Drapeau vert

    
      :DRAPEAU_ROUGE:
        Drapeau rouge

    


Structures
==========



.. c:type:: position

  .. code-block:: c

    struct position {
        int ligne;
        int colonne;
    };

  :Description: Position dans la mine, donnée par deux coordonnées.

  :Champs:
    :ligne: Coordonnée : ligne
    :colonne: Coordonnée : colonne
    


.. c:type:: minerai

  .. code-block:: c

    struct minerai {
        int resistance;
        int rendement;
    };

  :Description: Minerai à récolter

  :Champs:
    :resistance: Nombre de coups de pioches encore nécessaires avant que le bloc de minerais ne casse
    :rendement: Valeur marchande du bloc de minerai
    


.. c:type:: nain

  .. code-block:: c

    struct nain {
        position pos;
        int vie;
        int pa;
        int pm;
        bool accroche;
        int butin;
    };

  :Description: Nain (standard)

  :Champs:
    :pos: Position actuelle du nain (standard)
    :vie: Point(s) de vie restant du nain (standard)
    :pa: Point(s) d'action restant du nain (standard)
    :pm: Point(s) de déplacement restant du nain (standard)
    :accroche: Le nain (standard) est accroché à la paroi ou à une corde
    :butin: Valeur marchande totale que le nain (standard) possède
    


.. c:type:: action_hist

  .. code-block:: c

    struct action_hist {
        action_type atype;
        int id_nain;
        direction dir;
        direction sens;
    };

  :Description: Action de déplacement représentée dans l'historique.

  :Champs:
    :atype: Type de l'action
    :id_nain: Numéro du nain (standard) concerné par l'action
    :dir: Direction visée par le nain (standard) durant le déplacement
    :sens: Sens de l'action, utilisé uniquement pour préciser si l'on doit tirer une corde vers le bas ou vers le haut. Direction doit cibler la droite ou la gauche.
    


Fonctions
=========



.. c:function:: direction array chemin(position pos1, position pos2)

    Renvoie un chemin entre deux positions de la mine sous la forme d'une suite de directions à emprunter. Ce chemin minimise le nombre de blocs de granite nécessaire à casser. Si la position est invalide ou qu'il n'existe pas de tel chemin, le chemin renvoyé est vide.

    


    

    
      
    :param pos1: Position de départ
      
    
      
    :param pos2: Position d'arrivée
      
    


  


.. c:function:: erreur deplacer(int id_nain, direction dir)

    Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    
      
    :param dir: Direction visée
      
    


  


.. c:function:: erreur lacher(int id_nain)

    Le nain (standard) ``id_nain`` lâche la paroi.

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    


  


.. c:function:: erreur agripper(int id_nain)

    Le nain (standard) ``id_nain`` s'agrippe à la paroi.

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    


  


.. c:function:: erreur miner(int id_nain, direction dir)

    Le nain (standard) ``id_nain`` mine le bloc ou le nain (standard) dans la direction indiquée.

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    
      
    :param dir: Direction visée
      
    


  


.. c:function:: erreur tirer(int id_nain, direction dir_corde, direction sens)

    Le nain (standard) ``id_nain`` tire la corde dans le sens donné (HAUT ou BAS).

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    
      
    :param dir_corde: Direction dans laquelle se trouve la corde
      
    
      
    :param sens: Sens dans lequel le nain tire sur la corde
      
    


  


.. c:function:: erreur poser_corde(int id_nain, direction dir)

    Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    
      
    :param dir: Direction visée
      
    


  


.. c:function:: erreur debug_afficher_drapeau(position pos, debug_drapeau drapeau)

    Affiche le drapeau spécifié sur la case indiquée.

    


    

    
      
    :param pos: Case choisie
      
    
      
    :param drapeau: Drapeau à afficher sur la case
      
    


  


.. c:function:: case_type type_case(position pos)

    Renvoie le type d'une case donnée.

    


    

    
      
    :param pos: Case choisie
      
    


  


.. c:function:: position array liste_cordes()

    Renvoie la liste de toutes les positions occupées par une corde dans la mine.

    



  


.. c:function:: bool corde_sur_case(position pos)

    Indique si une corde se trouve sur une case donnée.

    


    

    
      
    :param pos: Case choisie
      
    


  


.. c:function:: int nain_sur_case(position pos)

    Renvoie le numéro du joueur à qui appartienent les nains (standard) sur la case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est invalide.

    


    

    
      
    :param pos: Case choisie
      
    


  


.. c:function:: nain info_nain(int id_joueur, int id_nain)

    Renvoie la description du nain (standard) désigné par le numéro ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas présent sur la carte, tous les membres de la structure ``nain`` renvoyée sont initialisés à -1 (et le champ ``accroche`` à `false`).

    


    

    
      
    :param id_joueur: Numéro du joueur
      
    
      
    :param id_nain: Numéro du nain (standard)
      
    


  


.. c:function:: position array liste_minerais()

    Renvoie la liste de tous les minerais dans la mine.

    



  


.. c:function:: minerai info_minerai(position pos)

    Renvoie la description d'un minerai en fonction d'une position donnée. Si le minerai n'est pas présent sur la carte, ou si la position est invalide, tous les membres de la structure ``minerai`` renvoyée sont initialisés à -1.

    


    

    
      
    :param pos: Case choisie
      
    


  


.. c:function:: int cout_deplacement(int id_nain, direction dir)

    Renvoie le nombre de points de déplacement que coûterai le déplacement d'un nain (standard) dans une direction donnée. Renvoie -1 si le déplacement n'est pas possible.

    


    

    
      
    :param id_nain: Numéro du nain (standard)
      
    
      
    :param dir: Direction visée
      
    


  


.. c:function:: position position_taverne(int id_joueur)

    Renvoie la position de la taverne appartenant au joueur ``id_joueur``. Si le joueur n'existe pas, renvoie la position (-1, -1).

    


    

    
      
    :param id_joueur: Numéro du joueur
      
    


  


.. c:function:: action_hist array historique()

    Renvoie la liste des actions effectuées par l’adversaire durant son tour, dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans cette liste.

    



  


.. c:function:: int score(int id_joueur)

    Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est invalide.

    


    

    
      
    :param id_joueur: Numéro du joueur
      
    


  


.. c:function:: int moi()

    Renvoie votre numéro de joueur.

    



  


.. c:function:: int adversaire()

    Renvoie le numéro de joueur de votre adversaire.

    



  


.. c:function:: bool annuler()

    Annule la dernière action. Renvoie faux quand il n'y a pas d'action à annuler ce tour ci.

    



  


.. c:function:: int tour_actuel()

    Retourne le numéro du tour actuel.

    



  


Fonctions utilisateur
=====================



.. c:function:: void partie_init()

    Fonction appelée au début de la partie.

    



  


.. c:function:: void jouer_tour()

    Fonction appelée à chaque tour.

    



  


.. c:function:: void partie_fin()

    Fonction appelée à la fin de la partie.

    



  

