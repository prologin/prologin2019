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
#include "api.hh"

#include <stdlib.h>

#include "actions.hh"

// Little helpers
inline bool dir_valid(direction dir)
{
    return 0 <= dir && dir < 4;
}

inline bool nain_valid(int nain_id)
{
    return 0 <= nain_id && nain_id < NB_NAINS;
}

inline bool player_valid(int player_id)
{
    return player_id != -1;
}

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state, rules::Player_sptr player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

/// Renvoie le plus court chemin entre deux positions de la mine sous la forme
/// d'une suite de direction à emprunter. Si la position est invalide ou que le
/// chemin n'existe pas, le chemin renvoyé est vide.
std::vector<direction> Api::chemin(position pos1, position pos2)
{
    if (!inside_map(pos1) || !inside_map(pos2))
        return {};

    return game_state_->map().get_shortest_path(pos1, pos2);
}

/// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
erreur Api::deplacer(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionDeplacer(id_nain, dir, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Le nain (standard) ``id_nain`` lâche la paroi.
erreur Api::lacher(int id_nain)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionLacher(id_nain, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Le nain (standard) ``id_nain`` s'agrippe à la paroi.
erreur Api::agripper(int id_nain)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionAgripper(id_nain, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Le nain (standard) ``id_nain`` creuse ou mine le bloc ou le nain (standard)
/// dans la direction indiquée.
erreur Api::miner(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionMiner(id_nain, dir, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Le nain (standard) ``id_nain`` tire dans un sens sur la corde.
erreur Api::tirer(int id_nain, direction dir_corde, direction sens)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(
        new ActionTirer(id_nain, dir_corde, sens, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.
erreur Api::poser_corde(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(new ActionPoserCorde(id_nain, dir, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Affiche le drapeau spécifié sur la case indiquée.
erreur Api::debug_afficher_drapeau(position pos, debug_drapeau drapeau)
{
    const int player_id = game_state_->get_player_id(player_->id);
    rules::IAction_sptr action(
        new ActionDebugAfficherDrapeau(pos, drapeau, player_id));

    erreur err;
    if ((err = static_cast<erreur>(game_state_check(action))) != OK)
        return err;

    actions_.add(action);
    game_state_save();
    game_state_apply(action);
    return OK;
}

/// Renvoie le type d'une case donnée.
case_type Api::type_case(position pos)
{
    if (!inside_map(pos))
        return ERREUR_CASE;

    return game_state_->map().get_cell_type(pos);
}

/// Renvoie la liste de toutes les positions occupées par une corde dans la
/// mine.
std::vector<position> Api::liste_cordes()
{
    return game_state_->map().get_ropes_positions();
}

/// Indique si une corde se trouve sur une case donnée.
bool Api::corde_sur_case(position pos)
{
    if (!inside_map(pos))
        return false;

    return game_state_->map().has_rope_at(pos);
}

/// Renvoie le numéro du joueur à qui appartient le nain (standard) sur la case
/// indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est
/// invalide.
int Api::nain_sur_case(position pos)
{
    if (!inside_map(pos))
        return -1;

    const int occupant = game_state_->map().get_cell_occupant(pos);

    if (occupant == -1)
        return -1;

    return game_state_->get_player_key(occupant);
}

/// Renvoie la description du nain (standard) désigné par le numéro ``id_nain``
/// appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas
/// présent sur la carte, tous les membres de la structure ``nain`` renvoyée
/// sont initialisés à -1 (et le champ ``accroche`` à `false`).
nain Api::info_nain(int id_joueur, int id_nain)
{
    const int player_id = game_state_->get_player_id(id_joueur);
    static const nain failure_ret = {{-1, -1}, -1, -1, -1, false, -1};

    if (!player_valid(player_id) || !nain_valid(id_nain))
        return failure_ret;

    const nain nain = game_state_->get_nain(player_id, id_nain);

    if (nain.vie <= 0)
        return failure_ret;

    return nain;
}

/// Renvoie la description d'un minerai en fonction d'une position donnée. Si le
/// minerai n'est pas présent sur la carte, ou si la position est invalide, tous
/// les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
minerai Api::info_minerai(position pos)
{
    if (!inside_map(pos))
        return {-1, -1};

    return game_state_->map().get_minerai_at(pos);
}

/// Renvoie la liste de tout les minerais dans la mine.
std::vector<position> Api::liste_minerais()
{
    return game_state_->map().get_ores();
}

/// Renvoie le nombre de points de déplacement pour le déplacement d'un nain
/// (standard) dans une direction donnée. Renvoie -1 si le déplacement n'est
/// pas possible.
int Api::cout_de_deplacement(int id_nain, direction dir)
{
    const int player_id = game_state_->get_player_id(player_->id);

    if (!nain_valid(id_nain) || !dir_valid(dir))
        return -1;

    const nain nain = game_state_->get_nain(player_id, id_nain);
    const position dest = get_position_offset(nain.pos, dir);

    if (!inside_map(dest) || game_state_->map().get_cell_type(dest) != LIBRE ||
        game_state_->map().get_cell_occupant(dest) ==
            game_state_->get_opponent_id(player_id))
        return -1;

    if (nain.vie <= 0)
        return -1;

    return game_state_->get_movement_cost(player_id, id_nain, dir);
}

/// Renvoie la position de la taverne appartenant au joueur ``id_joueur``. Si le
/// joueur n'existe pas, renvoie la position (-1, -1).
position Api::position_taverne(int id_joueur)
{
    const int player_id = game_state_->get_player_id(id_joueur);

    if (!player_valid(player_id))
        return {-1, -1};

    return game_state_->map().get_spawn_point(player_id);
}

/// Renvoie la liste des actions effectuées par l’adversaire durant son tour,
/// dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans
/// cette liste.
std::vector<action_hist> Api::historique()
{
    return game_state_->get_history(game_state_->get_player_id(adversaire()));
}

/// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est
/// invalide.
int Api::score(int id_joueur)
{
    const int player_id = game_state_->get_player_id(id_joueur);

    if (!player_valid(id_joueur))
        return -1;

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

/// Retourne le numéro du tour actuel.
int Api::tour_actuel()
{
    return game_state_->get_round();
}
