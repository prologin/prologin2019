#include "actions.hh"

#include <algorithm>

#include "position.hh"

int ActionTirer::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;
    if (dir_corde_ < 0 || dir_corde_ >= 4)
        return DIRECTION_INVALIDE;
    if (sens_ != HAUT && sens_ != BAS)
        return DIRECTION_INVALIDE;

    const nain* nain = st->get_nain(player_id_, id_nain_);
    if (nain == nullptr)
        return NAIN_MORT;
    if (nain->accroche)
        return DEJA_ACCROCHE;
    position dest = get_position_offset(nain->pos, dir_corde_);
    if (!inside_map(dest))
        return HORS_LIMITES;
    if (st->get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;
    const Rope *rope = st->get_rope(dest);
    if (rope == nullptr)
        return -1; // TODO no rope
    if (rope->get_nains().empty())
        return PAS_DE_NAIN;

    return OK;
}

void ActionTirer::apply_on(GameState* st) const
{
    const nain* nain = st->get_nain(player_id_, id_nain_);
    st->reduce_pa(player_id_, id_nain_, COUT_TIRER);
    position dest = get_position_offset(nain->pos, dir_corde_);

    const Rope *rope = st->get_rope(dest);
    auto nains = rope->get_nains();
    std::sort(nains.begin(), nains.end(), [&] (const auto& a, const auto& b)
        {
            position pa = st->get_nain(a.first, a.second)->pos;
            position pb = st->get_nain(b.first, b.second)->pos;
            if (sens_ == HAUT)
                return pa.ligne < pb.ligne;
            return pb.ligne < pa.ligne;
        });

    for (auto& nain : nains)
    {
        position pos = st->get_nain(nain.first, nain.second)->pos;
        position dest = get_position_offset(pos, sens_);
        if (st->get_rope(dest) == nullptr)
            continue;
        int ownership = st->get_internal_cell_ownership(dest);
        if (ownership != player_id_ && ownership != -1)
            continue;
        st->set_nain_position_internal(player_id_, id_nain_, dest);
    }

    internal_action action;
    action.internal = false;
    action.action = { ACTION_TIRER, id_nain_, dir_corde_, sens_ };
    st->add_to_internal_history(player_id_, action);
}
