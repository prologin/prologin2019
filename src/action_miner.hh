#ifndef ACTION_MINER_HH
#define ACTION_MINER_HH

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionMiner : public rules::Action<GameState>
{
public:
    ActionMiner(int id_nain, direction dir, int player_id)
        : id_nain_(id_nain)
        , dir_(dir)
        , player_id_(player_id)
    {
    }
    ActionMiner() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(id_nain_);
        buf.handle(dir_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_MINER; }

private:
    int id_nain_;
    direction dir_;
    int player_id_;
};

#endif // !ACTION_MINER_HH
