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

#include <stdlib.h>

#include "actions.hh"
#include "api.hh"

// global used in interface.cc
Api* api;

Api::Api(GameState* game_state, rules::Player_sptr player)
    : game_state_(game_state)
    , player_(player)
{
    api = this;
}

/// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
erreur Api::deplacer(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionDeplacer(id_nain, dir, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` lâche la paroi.
erreur Api::lacher(int id_nain)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionLacher(id_nain, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` s'agrippe à la paroi.
erreur Api::agripper(int id_nain)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionAgripper(id_nain, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` creuse ou mine le bloc ou le nain (standard)
/// dans la direction indiquée.
erreur Api::miner(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionMiner(id_nain, dir, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` tire dans un sens sur la corde.
erreur Api::tirer(int id_nain, direction dir_corde, direction sens)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(
        new ActionTirer(id_nain, dir_corde, sens, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.
erreur Api::poser_corde(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionPoserCorde(id_nain, dir, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Affiche le drapeau spécifié sur la case indiquée.
erreur Api::debug_afficher_drapeau(position pos, debug_drapeau drapeau)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(
        new ActionDebugAfficherDrapeau(pos, drapeau, player_id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Renvoie le type d'une case donnée.
case_type Api::type_case(position pos)
{
    return game_state_->get_cell_type(pos);
}

/// Renvoie la liste de toutes les corde dans la mine.
std::vector<position> Api::liste_cordes()
{
    return game_state_->get_ropes();
}

/// Indique si une corde se trouve sur une case donnée.
bool Api::corde_sur_case(position pos)
{
    return game_state_->has_rope_at(pos);
}

/// Renvoie le numéro du joueur à qui appartient le nain (standard) sur la case
/// indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est
/// invalide.
int Api::nain_sur_case(position pos)
{
    const int occupant = game_state_->get_cell_occupant(pos);

    if (occupant == -1)
        return -1;

    return game_state_->get_player_key(occupant);
}

/// Renvoie la description du nain (standard) désigné par le numéro ``id_nain``
/// appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas
/// présent sur la carte, tous les membres de la structure ``nain`` renvoyée
/// sont initialisés à -1.
nain Api::info_nain(int id_joueur, int id_nain)
{
    if ((id_joueur != moi() && id_joueur != adversaire()) ||
        (id_nain < 0 || id_nain >= NB_NAINS))
        return {{-1, -1}, -1, -1, -1, false, -1};

    const int player_id = game_state_->get_player_id(id_joueur);
    return game_state_->get_nain(player_id, id_nain);
}

/// Renvoie la description d'un minerai en fonction d'une position donnée. Si le
/// minerai n'est pas présent sur la carte, ou si la position est invalide, tous
/// les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
minerai Api::info_minerai(position pos)
{
    return game_state_->get_minerai_at(pos);
}

/// Renvoie la liste de tout les minerais dans la mine.
std::vector<position> Api::liste_minerais()
{
    return game_state_->get_ores();
}

/// Renvoie le nombre de points de déplacement pour le déplacement d'un nain
/// (standard) dans une direction donnée.
int Api::cout_de_deplacement(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    return game_state_->get_movement_cost(player_id, id_nain, dir);
}

/// Renvoie la position de la taverne appartenant au joueur ``id_joueur``. Si le
/// joueur n'existe pas, renvoie la position (-1, -1).
position Api::position_taverne(int id_joueur)
{
    if (id_joueur != moi() && id_joueur != adversaire())
        return {-1, -1};

    const int player_id = game_state_->get_player_id(id_joueur);
    return game_state_->get_spawn_point(player_id);
}

/// Renvoie le plus court chemin entre deux positions de la mine sous la forme
/// d'une suite de direction à emprunter. Si la position est invalide ou que le
/// chemin n'existe pas, le chemin renvoyé est vide.
std::vector<direction> Api::chemin(position pos1, position pos2)
{
    return game_state_->get_shortest_path(pos1, pos2);
}

/// Renvoie la liste des actions effectuées par l’adversaire durant son tour,
/// dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans
/// cette liste.
std::vector<action_hist> Api::historique()
{
    return game_state_->get_history(adversaire());
}

/// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est
/// invalide.
int Api::score(int id_joueur)
{
    if (id_joueur != moi() && id_joueur != adversaire())
        return -1;

    const int player_id = game_state_->get_player_id(id_joueur);
    return game_state_->get_score(player_id);
}

/// Renvoie votre numéro de joueur.
int Api::moi()
{
    return player_->id;
}

/// Renvoie le numéro de joueur de votre adversaire.
int Api::adversaire()
{
    const int player_id = game_state_->get_player_id(moi());
    return game_state_->get_player_key(game_state_->get_opponent_id(player_id));
}

/// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à
/// annuler ce tour ci.
bool Api::annuler()
{
    if (!game_state_->can_cancel())
        return false;

    actions_.cancel();
    game_state_ = rules::cancel(game_state_);
    return true;
}

/// Retourne le numéro du tour actuel.
int Api::tour_actuel()
{
    return game_state_->get_round();
}
