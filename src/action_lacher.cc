#include "actions.hh"

int ActionLacher::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain& nain = st->get_nain(player_id_, id_nain_);
    if (!nain.accroche)
        return -1; // TODO error le nain (standard) n'est pas accroche

    return OK;
}

void ActionLacher::apply_on(GameState* st) const
{
    // FIXME
}
