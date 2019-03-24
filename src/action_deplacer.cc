#include "actions.hh"

#include "position.hh"

int ActionDeplacer::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return -1; // TODO nain introuvable
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    NainInfo nain = st->get_nain_info(player_id_, id_nain_);

    position start = nain.get_position();
    position dest = get_position_offset(start, dir_);

    if (!inside_map(dest))
        return DEPLACEMENT_HORS_LIMITES;
    if (st->get_cell_type(dest) != LIBRE)
        return -1; //TODO dans un mur
    
    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    NainInfo nain = st->get_nain_info(player_id_, id_nain_);

    position start = nain.get_position();
    position dest = get_position_offset(start, dir_);

    st->set_nain_position(player_id_, id_nain_, dest);
}
