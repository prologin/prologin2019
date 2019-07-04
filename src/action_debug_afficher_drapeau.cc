#include "actions.hh"

#include "position.hh"

int ActionDebugAfficherDrapeau::check(const GameState& st) const
{
    if (!st.is_init())
        FATAL("action: you cannot use action outside jouer_tour");

    if (!inside_map(pos_))
        return HORS_LIMITES;

    if (drapeau_ < 0 || drapeau_ > 3)
        return DRAPEAU_INVALIDE;

    return OK;
}

void ActionDebugAfficherDrapeau::apply_on(GameState* st) const
{
    auto player_id_ = st->get_player_id(this->player_id_);

    internal_action action;
    action.type = 0;
    action.debug_flag = {drapeau_, pos_};
    st->add_to_internal_history(player_id_, action);
}
