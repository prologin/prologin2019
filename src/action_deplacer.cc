#include "actions.hh"

#include "position.hh"

int ActionDeplacer::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    const nain& nain = st->get_nain(player_id_, id_nain_);

    if (nain.pm < COUT_DEPLACEMENT)
        return PM_INSUFFISANTS;

    position dest = get_position_offset(nain.pos, dir_);

    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_cell_type(dest) != LIBRE)
        return -1; //TODO dans un mur
    
    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    const nain& nain = st->get_nain(player_id_, id_nain_);

    position dest = get_position_offset(nain.pos, dir_);

    st->set_nain_position(player_id_, id_nain_, dest);
    st->reduce_pm(player_id_, id_nain_, COUT_DEPLACEMENT);
}
