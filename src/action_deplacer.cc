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
    
    if (nain->pm < 1) // TODO cout_deplacement
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
    st->reduce_pm(player_id_, id_nain_, 1); // TODO cout_deplacement
    st->set_nain_position(player_id_, id_nain_, dest);
    st->check_gravity(dest);
}
