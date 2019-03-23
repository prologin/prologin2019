#include "actions.hh"

int ActionDeplacer::check(const GameState* st) const
{
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return -1; // TODO nain introuvable
    if (dir_ < 0 || dir_ >= 4)
        return DIRECTION_INVALIDE;

    
    // FIXME
    return OK;
}

void ActionDeplacer::apply_on(GameState* st) const
{
    // FIXME
}
