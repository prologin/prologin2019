#include "actions.hh"

#include "position.hh"

int ActionMiner::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_ < 0 ||  dir_ >= 4)
        return DIRECTION_INVALIDE;

    const nain* nain = st->get_nain(player_id_, id_nain_);
    if (nain == nullptr)
        return -1; // TODO le nain (standard) est mort
    position dest = get_position_offset(nain->pos, dir_);
    if (!inside_map(dest))
        return HORS_LIMITES;
    case_type type = st->get_cell_type(dest);
    if (type == OBSIDIENNE)
        return OBSTACLE_MUR; // TODO maybe change error

    const auto& nains = st->get_nains_at(dest);
    if (type == LIBRE && nains.second.empty())
        return -1; // TODO pas de nains a taper

    if (nain->pa < COUT_MINER)
        return PA_INSUFFISANTS;

    return OK;
}

void ActionMiner::apply_on(GameState* st) const
{
    const nain* nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain->pos, dir_);
    case_type type = st->get_cell_type(dest);
    if (type == LIBRE)
    {
        const auto& nains = st->get_nains_at(dest);
        for (int nain_id : nains.second)
            st->reduce_pv(nains.first, nain_id, 3); // TODO add constant DEGAT_PIOCHE
        return;
    }
    const minerai *minerai = st->get_minerai(dest);
    if (minerai != nullptr)
        if (!st->mine_minerai(dest, player_id_, id_nain_))
            return;
    st->set_cell_type(dest, LIBRE);
    position up = get_position_offset(dest, HAUT);
    if (inside_map(up))
        st->check_gravity(up);
}
