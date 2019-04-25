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

#include <vector>
#include <rules/game-state.hh>
#include <rules/player.hh>
#include <rules/actions.hh>

#include "game_state.hh"
#include "constant.hh"

/*!
** The methods of this class are exported through 'interface.cc'
** to be called by the clients
*/
class Api
{

public:
    Api(GameState* game_state, rules::Player_sptr player);
    virtual ~Api() { }

    const rules::Player_sptr player() const { return player_; }
    void player_set(rules::Player_sptr player) { player_ = player; }

    rules::Actions* actions() { return &actions_; }

    const GameState* game_state() const { return game_state_; }
    GameState* game_state() { return game_state_; }
    void game_state_set(rules::GameState* gs) {
        game_state_ = dynamic_cast<GameState*>(gs);
    }

private:
    GameState* game_state_;
    rules::Player_sptr player_;
    rules::Actions actions_;

public:

    /// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
    erreur deplacer(int id_nain, direction dir);

    /// Le nain (standard) ``id_nain`` lâche la paroi ou la corde.
    erreur lacher(int id_nain);

    /// Le nain (standard) ``id_nain`` creuse ou mine le bloc ou le nain (standard) dans la direction indiquée.
    erreur miner(int id_nain, direction dir);

    /// Le nain (standard) ``id_nain`` tire dans un sens sur la corde.
    erreur tirer(int id_nain, direction dir_corde, direction sens);

    /// Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.
    erreur poser_corde(int id_nain, direction dir);

    /// Affiche le drapeau spécifié sur la case indiquée.
    erreur debug_afficher_drapeau(position pos, debug_drapeau drapeau);

    /// Renvoie le type d'une case donnée.
    case_type type_case(position pos);

    /// Renvoie la liste de toutes les corde dans la mine.
    std::vector<position> liste_cordes();

    /// Indique si une corde se trouve sur une case donnée.
    bool corde_sur_case(position pos);

    /// Renvoie le numéro du joueur à qui appartient le nain (standard) sur la case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est invalide.
    int nain_sur_case(position pos);

    /// Renvoie la description du nain (standard) désigné par le numéro ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas présent sur la carte, tous les membres de la structure ``nain`` renvoyée sont initialisés à -1.
    nain info_nain(int id_joueur, int id_nain);

    /// Renvoie la description d'un minerai en fonction d'une position donnée. Si le minerai n'est pas présent sur la carte, ou si la position est invalide, tous les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
    minerai info_minerai(position pos);

    /// Renvoie le nombre de points de déplacement pour le déplacement d'un nain (standard) dans une direction donnée.
    int cout_deplacement(int id_nain, direction dir);

    /// Renvoie la liste des actions effectuées par l’adversaire durant son tour, dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans cette liste.
    std::vector<action_hist> historique();

    /// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est invalide.
    int score(int id_joueur);

    /// Renvoie votre numéro de joueur.
    int moi();

    /// Renvoie le numéro de joueur de votre adversaire.
    int adversaire();

    /// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à annuler ce tour-ci.
    bool annuler();

    /// Retourne le numéro du tour actuel.
    int tour_actuel();

};


#endif /* !API_HH_ */
