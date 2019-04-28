#include "actions.hh"

#include "position.hh"

int ActionDebugAfficherDrapeau::check(const GameState* /* st */) const
{
    if (!inside_map(pos_))
        return POSITION_INVALIDE;
    if (drapeau_ < 0 || drapeau_ > 3)
        return DRAPEAU_INVALIDE;
    return OK;
}

void ActionDebugAfficherDrapeau::apply_on(GameState* st) const
{
    internal_action action;
    action.internal = true;
    action.debug_flag = { drapeau_, pos_ };
    st->add_to_internal_history(player_id_, action);
}
