#include "actions.hh"

int ActionTirer::check(const GameState* st) const
{
    // FIXME
    return 0;
}

void ActionTirer::apply_on(GameState* st) const
{
    // FIXME

    internal_action action;
    action.internal = false;
    action.action = { ACTION_TIRER, id_nain_, dir_corde_, sens_ };
    st->add_to_internal_history(player_id_, action);
}
