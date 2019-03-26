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

void ActionDebugAfficherDrapeau::apply_on(GameState* /* st */) const
{
}
