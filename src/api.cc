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

#include "api.hh"
#include "actions.hh"

// global used in interface.cc
Api* api;

Api::Api(GameState* game_state, rules::Player_sptr player)
    : game_state_(game_state),
      player_(player)
{
    api = this;
}


/// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
erreur Api::deplacer(int id_nain, direction dir)
{
    rules::IAction_sptr action(new ActionDeplacer(id_nain, dir, player_->id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` s'accroche à la paroi et grimpe dans la direction choisie.
erreur Api::escalader(int id_nain, direction dir)
{
    rules::IAction_sptr action(new ActionEscalader(id_nain, dir, player_->id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` s'accroche à la corde et se déplace le long de la corde.
erreur Api::grimper(int id_nain, direction dir)
{
    rules::IAction_sptr action(new ActionGrimper(id_nain, dir, player_->id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` lâche la paroi ou la corde.
erreur Api::lacher(int id_nain)
{
    rules::IAction_sptr action(new ActionLacher(id_nain, player_->id));

    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state_));
    return OK;
}

/// Le nain (standard) ``id_nain`` creuse ou mine le bloc ou le nain (standard) dans la direction indiquée.
erreur Api::miner(int id_nain, direction dir)
{
    rules::IAction_sptr action(new ActionMiner(id_nain, dir, player_->id));

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
    rules::IAction_sptr action(new ActionTirer(id_nain, dir_corde, sens, player_->id));

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
    rules::IAction_sptr action(new ActionPoserCorde(id_nain, dir, player_->id));

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
    rules::IAction_sptr action(new ActionDebugAfficherDrapeau(pos, drapeau, player_->id));

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
    return game_state_->get_rope(pos) != nullptr;
}

/// Renvoie le numéro du joueur à qui appartient le nain (standard) sur la case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est invalide.
int Api::nain_sur_case(position pos)
{
    // TODO
    abort();
}

/// Renvoie la description du nain (standard) désigné par le numéro ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas présent sur la carte, tous les membres de la structure ``nain`` renvoyée sont initialisés à -1.
nain Api::info_nain(int id_joueur, int id_nain)
{
    if (id_nain < 0 || id_nain >= NB_NAINS)
        return { { -1, -1 }, -1, -1, -1, -1 };
    if (id_joueur != moi() && id_joueur != adversaire())
        return { { -1, -1 }, -1, -1, -1, -1 };
    return game_state_->get_nain(id_joueur, id_nain);
}

/// Renvoie la description d'un minerai en fonction d'une position donnée. Si le minerai n'est pas présent sur la carte, ou si la position est invalide, tous les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
minerai Api::info_minerrai(position pos)
{
    return game_state_->get_minerrai(pos);
}

/// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est invalide.
int Api::score(int id_joueur)
{
    if (id_joueur != moi() && id_joueur != adversaire())
        return -1;
    return game_state_->get_score(id_joueur);
}

/// Renvoie votre numéro de joueur.
int Api::moi()
{
    return player_->id;
}

/// Renvoie le numéro de joueur de votre adversaire.
int Api::adversaire()
{
    return game_state_->opponent(moi());
}

/// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à annuler ce tour-ci.
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


