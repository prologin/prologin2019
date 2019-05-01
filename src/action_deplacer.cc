#include "actions.hh"

#include "position.hh"

int ActionDeplacer::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    const nain* nain = st->get_nain(player_id_, id_nain_);
    if (nain == nullptr)
        return NAIN_MORT;

    if (nain->pm < st->get_movement_cost(player_id_, id_nain_, dir_))
        return PM_INSUFFISANTS;

    position dest = get_position_offset(nain->pos, dir_);
    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;
    int dest_owner = st->get_cell_ownership(dest);
    if (dest_owner != -1 && dest_owner != player_id_)
        return OBSTACLE_NAIN;

    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    const nain* nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain->pos, dir_);
    position up = get_position_offset(nain->pos, HAUT);
    st->reduce_pm(player_id_, id_nain_, st->get_movement_cost(player_id_, id_nain_, dir_));
    st->set_nain_position(player_id_, id_nain_, dest);
    st->check_gravity(dest, player_id_);
    if (inside_map(up))
        st->check_gravity(up, player_id_);

    internal_action action;
    action.type = 1;
    action.action = { ACTION_DEPLACER, id_nain_, dir_, ERREUR_DIRECTION };
    st->add_to_internal_history(player_id_, action);
}
