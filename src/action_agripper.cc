#include "actions.hh"

int ActionAgripper::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);

    if (nain.vie <= 0)
        return NAIN_MORT;

    if (nain.pa < COUT_AGRIPPER)
        return PA_INSUFFISANTS;

    if (nain.accroche)
        return DEJA_ACCROCHE;

    return OK;
}

void ActionAgripper::apply_on(GameState* st) const
{
    st->set_nain_accroche(player_id_, id_nain_, true);
    st->reduce_pa(player_id_, id_nain_, COUT_AGRIPPER);

    const nain nain = st->get_nain(player_id_, id_nain_);

    if (st->map().has_rope_at(nain.pos))
        st->map().add_nain_to_rope(nain.pos, player_id_, id_nain_);

    internal_action action;
    action.type = 1;
    action.action = {ACTION_AGRIPPER, id_nain_, ERREUR_DIRECTION,
                     ERREUR_DIRECTION};
    st->add_to_internal_history(player_id_, action);
}
