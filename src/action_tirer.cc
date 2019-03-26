#include "actions.hh"

#include "position.hh"

int ActionTirer::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_corde_ < 0 || dir_corde_ >= 4)
        return DIRECTION_INVALIDE;

    const nain& nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain.pos, dir_corde_);

    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_cell_type(dest) != LIBRE)
        return -1; //TODO dans un mur
    if (!st->is_rope(dest))
        return -1; //TODO pas une corde
    return OK;
}

void ActionTirer::apply_on(GameState* st) const
{
    // FIXME
}
