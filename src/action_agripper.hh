#ifndef ACTION_AGRIPPER_HH
#define ACTION_AGRIPPER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionAgripper : public rules::Action<GameState>
{
public:
    ActionAgripper(int id_nain, int player_id) : id_nain_(id_nain), player_id_(player_id) {}
    ActionAgripper() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_nain_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_AGRIPPER; }

private:
    int id_nain_;
    int player_id_;
};

#endif // !ACTION_AGRIPPER_HH
