#include "actions.hh"

int ActionLacher::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);

    if (nain.vie <= 0)
        return NAIN_MORT;

    if (nain.pa < COUT_LACHER)
        return PA_INSUFFISANTS;

    if (!nain.accroche)
        return PAS_ACCROCHE;

    return OK;
}

void ActionLacher::apply_on(GameState* st) const
{
    internal_action action;
    action.type = 1;
    action.action = {ACTION_LACHER, id_nain_, ERREUR_DIRECTION,
                     ERREUR_DIRECTION};
    st->add_to_internal_history(player_id_, action);

    st->set_nain_accroche(player_id_, id_nain_, false);
    st->reduce_pa(player_id_, id_nain_, COUT_LACHER);
}
