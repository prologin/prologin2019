#ifndef ACTION_TIRER_HH
#define ACTION_TIRER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "game_state.hh"
#include "constant.hh"

class ActionTirer : public rules::Action<GameState>
{
public:
    ActionTirer(int id_nain, direction dir_corde, direction sens, int player_id) : id_nain_(id_nain), dir_corde_(dir_corde), sens_(sens), player_id_(player_id) {}
    ActionTirer() {} // for register_action()

    int check(const GameState* st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_nain_);
        buf.handle(dir_corde_);
        buf.handle(sens_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_TIRER; }

private:
    int id_nain_;
    direction dir_corde_;
    direction sens_;
    int player_id_;
};

#endif // !ACTION_TIRER_HH
