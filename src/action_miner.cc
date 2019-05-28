#include "actions.hh"

#include "position.hh"

int ActionMiner::check(const GameState* st) const
{
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    // Check nain
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);
    if (nain.vie <= 0)
        return NAIN_MORT;

    if (nain.pa < COUT_MINER)
        return PA_INSUFFISANTS;

    // Check position
    position dest = get_position_offset(nain.pos, dir_);

    if (!inside_map(dest))
        return HORS_LIMITES;

    case_type type = st->map().get_cell_type(dest);

    if (type == OBSIDIENNE)
        return OBSTACLE_MUR;

    if (type == LIBRE && st->map().get_cell_occupant(dest) == -1)
        return PAS_DE_NAIN;

    return OK;
}

void ActionMiner::apply_on(GameState* st) const
{
    internal_action action;
    action.type = 1;
    action.action = {ACTION_MINER, id_nain_, dir_, ERREUR_DIRECTION};
    st->add_to_internal_history(player_id_, action);

    const nain nain = st->get_nain(player_id_, id_nain_);
    st->reduce_pa(player_id_, id_nain_, COUT_MINER);
    position dest = get_position_offset(nain.pos, dir_);
    case_type type = st->map().get_cell_type(dest);

    if (type == LIBRE)
    {
        // Handle an attack
        const int player_id = st->map().get_cell_occupant(dest);
        const auto nains_ids = st->map().get_nains_ids_at(dest);

        for (int nain_id : nains_ids)
            st->reduce_pv(player_id, nain_id, DEGAT_PIOCHE, player_id_);
    }
    else
    {
        // Handle mining
        if (st->map().has_minerai_at(dest))
            st->mine_minerai(dest, player_id_, id_nain_);
        else
            st->set_cell_type(dest, LIBRE, player_id_);
    }
}
