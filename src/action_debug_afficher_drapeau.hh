#ifndef ACTION_DEBUG_AFFICHER_DRAPEAU_HH
#define ACTION_DEBUG_AFFICHER_DRAPEAU_HH

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionDebugAfficherDrapeau : public rules::Action<GameState>
{
public:
    ActionDebugAfficherDrapeau(position pos, debug_drapeau drapeau,
                               int player_id)
        : pos_(pos)
        , drapeau_(drapeau)
        , player_id_(player_id)
    {
    }
    ActionDebugAfficherDrapeau() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(pos_);
        buf.handle(drapeau_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_DEBUG_AFFICHER_DRAPEAU; }

private:
    position pos_;
    debug_drapeau drapeau_;
    int player_id_;
};

#endif // !ACTION_DEBUG_AFFICHER_DRAPEAU_HH
