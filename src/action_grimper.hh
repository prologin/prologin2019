#ifndef ACTION_GRIMPER_HH
#define ACTION_GRIMPER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionGrimper : public rules::Action<GameState>
{
public:
    ActionGrimper(int id_nain, direction dir, int player_id) : id_nain_(id_nain), dir_(dir), player_id_(player_id) {}
    ActionGrimper() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_nain_);
        buf.handle(dir_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_GRIMPER; }

private:
    int id_nain_;
    direction dir_;
    int player_id_;
};

#endif // !ACTION_GRIMPER_HH
