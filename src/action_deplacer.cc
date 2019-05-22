#include "actions.hh"

#include "position.hh"

int ActionDeplacer::check(const GameState* st) const
{
    // Check dwarf
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain* nain = st->get_nain(player_id_, id_nain_);
    if (nain == nullptr)
        return NAIN_MORT;

    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    // Check cell
    position dest = get_position_offset(nain->pos, dir_);
    if (!inside_map(dest))
        return HORS_LIMITES;

    if (st->get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    int dest_owner = st->get_cell_occupant(dest);
    if (dest_owner != -1 && dest_owner != player_id_)
        return OBSTACLE_NAIN;

    // Check cost
    int cost = st->get_movement_cost(player_id_, id_nain_, dir_);
    assert(cost >= 0);

    if (nain->pm < cost)
        return PM_INSUFFISANTS;

    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    const nain* nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain->pos, dir_);

    st->reduce_pm(player_id_, id_nain_, st->get_movement_cost(player_id_, id_nain_, dir_));
    st->set_nain_position(player_id_, id_nain_, dest);
    st->check_nain_gravity(dest, player_id_);

    position up = get_position_offset(nain->pos, HAUT);
    if (inside_map(up))
        st->check_nain_gravity(up, player_id_);

    if (st->get_rope(nain->pos) != nullptr)
        st->remove_nain_from_rope(nain->pos, player_id_, id_nain_);
    if (st->get_rope(dest) != nullptr)
        st->add_nain_to_rope(dest, player_id_, id_nain_);

    internal_action action;
    action.type = 1;
    action.action = { ACTION_DEPLACER, id_nain_, dir_, ERREUR_DIRECTION };
    st->add_to_internal_history(player_id_, action);
}
