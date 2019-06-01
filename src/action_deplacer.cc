#include "actions.hh"

#include "position.hh"

int ActionDeplacer::check(const GameState* st) const
{
    if (!st->is_init())
        FATAL("action: you cannot use action outside jouer_tour");

    // Check dwarf
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);
    if (nain.vie <= 0)
        return NAIN_MORT;

    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    // Check cell
    const position dest = get_position_offset(nain.pos, dir_);

    if (!inside_map(dest))
        return HORS_LIMITES;

    if (st->map().get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    int dest_owner = st->map().get_cell_occupant(dest);
    if (dest_owner == st->get_opponent_id(player_id_))
        return OBSTACLE_NAIN;

    // Check cost
    int cost = st->get_movement_cost(player_id_, id_nain_, dir_);

    if (nain.pm < cost)
        return PM_INSUFFISANTS;

    // NOTE: Little easter egg: kill a dwarf going to enemy camp.
    // It still returns OK though, the movement is added to the history and the
    // dwarf instantly killed.

    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    internal_action action;
    action.type = 1;
    action.action = {ACTION_DEPLACER, id_nain_, dir_, ERREUR_DIRECTION};
    st->add_to_internal_history(player_id_, action);

    const nain nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain.pos, dir_);

    // Kill nain if on the opponent spawn
    if (dest == st->map().get_spawn_point(1 - player_id_))
    {
        if (nain.butin > 0)
            st->increase_score(1 - player_id_, nain.butin);
        st->reduce_pv(player_id_, id_nain_, VIE_NAIN, player_id_);
        return;
    }

    st->reduce_pm(player_id_, id_nain_,
                  st->get_movement_cost(player_id_, id_nain_, dir_));
    st->set_nain_position(player_id_, id_nain_, dest);
    st->check_nain_gravity(dest, player_id_);

    position up = get_position_offset(nain.pos, HAUT);
    if (inside_map(up))
        st->check_nain_gravity(up, player_id_);
}
