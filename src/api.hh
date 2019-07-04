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

#ifndef API_HH_
#define API_HH_

#include <rules/actions.hh>
#include <rules/api.hh>
#include <rules/game-state.hh>
#include <rules/player.hh>
#include <vector>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"
#include "position.hh"

/*!
** The methods of this class are exported through 'interface.cc'
** to be called by the clients
*/
class Api : public rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state, rules::Player_sptr player);
    virtual ~Api() {}

    /// Renvoie le plus court chemin entre deux positions de la mine sous la
    /// forme d'une suite de direction à emprunter. Si la position est invalide
    /// ou que le chemin n'existe pas, le chemin renvoyé est vide.
    std::vector<direction> chemin(position pos1, position pos2);

    /// Déplace le nain (standard) ``id_nain`` d'une case dans la direction
    /// choisie.
    ApiActionFunc<ActionDeplacer> deplacer{this};

    /// Le nain (standard) ``id_nain`` lâche la paroi.
    ApiActionFunc<ActionLacher> lacher{this};

    /// Le nain (standard) ``id_nain`` s'agrippe à la paroi.
    ApiActionFunc<ActionAgripper> agripper{this};

    /// Le nain (standard) ``id_nain`` creuse ou mine le bloc ou le nain
    /// (standard) dans la direction indiquée.
    ApiActionFunc<ActionMiner> miner{this};

    /// Le nain (standard) ``id_nain`` tire dans un sens sur la corde.
    ApiActionFunc<ActionTirer> tirer{this};

    /// Le nain (standard) ``id_nain`` pose une corde dans la direction
    /// indiquée.
    ApiActionFunc<ActionPoserCorde> poser_corde{this};

    /// Affiche le drapeau spécifié sur la case indiquée.
    ApiActionFunc<ActionDebugAfficherDrapeau> debug_afficher_drapeau{this};

    /// Renvoie le type d'une case donnée.
    case_type type_case(position pos);

    /// Renvoie la liste de toutes les positions occupées par une corde dans la
    /// mine.
    std::vector<position> liste_cordes();

    /// Indique si une corde se trouve sur une case donnée.
    bool corde_sur_case(position pos);

    /// Renvoie le numéro du joueur à qui appartient le nain (standard) sur la
    /// case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la
    /// position est invalide.
    int nain_sur_case(position pos);

    /// Renvoie la description du nain (standard) désigné par le numéro
    /// ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)
    /// n'est pas présent sur la carte, tous les membres de la structure
    /// ``nain`` renvoyée sont initialisés à -1 (et le champ ``accroche`` à
    /// `false`).
    nain info_nain(int id_joueur, int id_nain);

    /// Renvoie la description d'un minerai en fonction d'une position donnée.
    /// Si le minerai n'est pas présent sur la carte, ou si la position est
    /// invalide, tous les membres de la structure ``minerai`` renvoyée sont
    /// initialisés à -1.
    minerai info_minerai(position pos);

    /// Renvoie la liste de tout les minerais dans la mine.
    std::vector<position> liste_minerais();

    /// Renvoie le nombre de points de déplacement pour le déplacement d'un nain
    /// (standard) dans une direction donnée. Renvoie -1 si le déplacement n'est
    /// pas possible.
    int cout_de_deplacement(int id_nain, direction dir);

    /// Renvoie la position de la taverne appartenant au joueur ``id_joueur``.
    /// Si le joueur n'existe pas, renvoie la position (-1, -1).
    position position_taverne(int id_joueur);

    /// Renvoie la liste des actions effectuées par l’adversaire durant son
    /// tour, dans l'ordre chronologique. Les actions de débug n'apparaissent
    /// pas dans cette liste.
    std::vector<action_hist> historique();

    /// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est
    /// invalide.
    int score(int id_joueur);

    /// Renvoie votre numéro de joueur.
    int moi();

    /// Renvoie le numéro de joueur de votre adversaire.
    int adversaire();

    /// Retourne le numéro du tour actuel.
    int tour_actuel();
};

#endif /* !API_HH_ */
