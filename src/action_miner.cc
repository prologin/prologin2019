#include "actions.hh"

#include "position.hh"

int ActionMiner::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    const nain& nain = st->get_nain(player_id_, id_nain_);

    if (nain.pa < COUT_MINER)
        return PA_INSUFFISANTS;

    position dest = get_position_offset(nain.pos, dir_);

    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_cell_type(dest) != GRANITE)
        return -1; // TODO pas dans un mur

    return OK;
}

void ActionMiner::apply_on(GameState* st) const
{
    const nain& nain = st->get_nain(player_id_, id_nain_);

    position dest = get_position_offset(nain.pos, dir_);

    st->set_cell_type(dest, LIBRE);
    st->reduce_pa(player_id_, id_nain_, COUT_MINER);
}
