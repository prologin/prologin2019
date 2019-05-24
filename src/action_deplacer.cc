#include "actions.hh"

#include "position.hh"

int ActionDeplacer::check(const GameState* st) const
{
    // Check dwarf
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);
    if (nain.vie <= 0)
        return NAIN_MORT;

    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    // Check cell
    position dest = get_position_offset(nain.pos, dir_);
    if (!inside_map(dest))
        return HORS_LIMITES;

    if (st->map().get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    int dest_owner = st->map().get_cell_occupant(dest);
    if (dest_owner == st->get_opponent_id(player_id_))
        return OBSTACLE_NAIN;

    // Check cost
    int cost = st->get_movement_cost(player_id_, id_nain_, dir_);
    assert(cost >= 0);

    if (nain.pm < cost)
        return PM_INSUFFISANTS;

    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    const nain nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain.pos, dir_);

    st->reduce_pm(player_id_, id_nain_,
                  st->get_movement_cost(player_id_, id_nain_, dir_));
    st->set_nain_position(player_id_, id_nain_, dest);
    st->check_nain_gravity(dest, player_id_);

    if (st->map().has_rope_at(nain.pos))
        st->map().remove_nain_from_rope(nain.pos, player_id_, id_nain_);

    if (st->map().has_rope_at(dest))
        st->map().add_nain_to_rope(dest, player_id_, id_nain_);

    position up = get_position_offset(nain.pos, HAUT);
    if (inside_map(up))
        st->check_nain_gravity(up, player_id_);

    internal_action action;
    action.type = 1;
    action.action = {ACTION_DEPLACER, id_nain_, dir_, ERREUR_DIRECTION};
    st->add_to_internal_history(player_id_, action);
}
