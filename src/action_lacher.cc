#include "actions.hh"

int ActionLacher::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain* nain = st->get_nain(player_id_, id_nain_);
    if (nain == nullptr)
        return -1; // TODO le nain (standard) est mort
    if (!nain->accroche)
        return -1; // TODO error le nain (standard) n'est pas accroche

    return OK;
}

void ActionLacher::apply_on(GameState* st) const
{
    st->set_nain_accroche(player_id_, id_nain_, false);
    const nain* nain = st->get_nain(player_id_, id_nain_);
    st->check_gravity(nain->pos);
}
