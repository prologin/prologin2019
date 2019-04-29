#include "actions.hh"

#include "position.hh"

int ActionPoserCorde::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    for (int i = 0; i < NB_NAINS; ++i)
        if (st->get_nain(player_id_, i)->pa != NB_POINTS_ACTION)
            return PA_INSUFFISANTS;

    const nain* nain = st->get_nain(player_id_, id_nain_);
    if (nain == nullptr)
        return NAIN_MORT;
    position dest = get_position_offset(nain->pos, dir_);
    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_rope(dest) != nullptr)
        return OBSTACLE_CORDE;
    if (st->get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    return OK;
}

void ActionPoserCorde::apply_on(GameState* st) const
{
    const nain* nain = st->get_nain(player_id_, id_nain_);
    st->add_rope(get_position_offset(nain->pos, dir_));
    for (int n = 0; n < NB_NAINS; ++n)
    {
        int pa = st->get_nain(player_id_, n)->pa;
        st->reduce_pa(player_id_, n, pa);
    }

    internal_action action;
    action.internal = false;
    action.action = { ACTION_POSER_CORDE, id_nain_, dir_, ERREUR_DIRECTION };
    st->add_to_internal_history(player_id_, action);
}
