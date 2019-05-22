#ifndef ACTION_LACHER_HH
#define ACTION_LACHER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionLacher : public rules::Action<GameState>
{
public:
    ActionLacher(int id_nain, int player_id)
        : id_nain_(id_nain)
        , player_id_(player_id)
    {
    }
    ActionLacher() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_nain_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_LACHER; }

private:
    int id_nain_;
    int player_id_;
};

#endif // !ACTION_LACHER_HH
