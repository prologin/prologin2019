#include "actions.hh"

#include <algorithm>

#include "position.hh"

int ActionTirer::check(const GameState* st) const
{
    // Check nain
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);

    if (nain.vie <= 0)
        return NAIN_MORT;

    if (nain.accroche)
        return DEJA_ACCROCHE;

    if (nain.pa < COUT_TIRER)
        return PA_INSUFFISANTS;

    // Check positions
    if (dir_corde_ < 0 || dir_corde_ >= 4)
        return DIRECTION_INVALIDE;

    if (sens_ != HAUT && sens_ != BAS)
        return DIRECTION_INVALIDE;

    position dest = get_position_offset(nain.pos, dir_corde_);

    if (!inside_map(dest))
        return HORS_LIMITES;

    if (st->get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    // Check rope
    if (!st->has_rope_at(dest))
        return PAS_DE_CORDE;

    if (st->get_rope_at(dest).get_nains().empty())
        return PAS_DE_NAIN;

    return OK;
}

void ActionTirer::apply_on(GameState* st) const
{
    const nain nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain.pos, dir_corde_);

    st->reduce_pa(player_id_, id_nain_, COUT_TIRER);

    // Build the ordered list of dwarfs
    const Rope& rope = st->get_rope_at(dest);
    auto nains = rope.get_nains();

    std::sort(nains.begin(), nains.end(), [&](const auto& a, const auto& b) {
        position pa = st->get_nain(a.first, a.second).pos;
        position pb = st->get_nain(b.first, b.second).pos;

        if (sens_ == HAUT)
            return pa.ligne < pb.ligne;

        return pb.ligne < pa.ligne;
    });

    // Apply
    for (auto& nain : nains)
    {
        position pos = st->get_nain(nain.first, nain.second).pos;
        position dest = get_position_offset(pos, sens_);

        if (!st->has_rope_at(dest))
            continue;

        int occupant = st->get_cell_occupant(dest);
        if (occupant != st->get_opponent_id(player_id_))
            st->set_nain_position(nain.first, nain.second, dest);
    }

    internal_action action;
    action.type = 1;
    action.action = {ACTION_TIRER, id_nain_, dir_corde_, sens_};
    st->add_to_internal_history(player_id_, action);
}
