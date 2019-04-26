#include "actions.hh"

#include "position.hh"

int ActionPoserCorde::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;
    if (st->can_cancel())
        return PA_INSUFFISANTS; // TODO already do something

    const nain& nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain.pos, dir_);
    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_rope(dest) != nullptr)
        return -1; // TODO already a rope there
    if (st->get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    return OK;
}

void ActionPoserCorde::apply_on(GameState* st) const
{
    const nain& nain = st->get_nain(player_id_, id_nain_);
    Rope rope(get_position_offset(nain.pos, dir_));
    st->add_rope(rope);
}
