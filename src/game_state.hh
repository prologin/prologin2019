// FIXME License notice

#ifndef GAME_STATE_HH
#define GAME_STATE_HH

#include <unordered_map>

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "map.hh"
#include "player_info.hh"

class GameState : public rules::GameState
{
    public:
        GameState(std::istream& map_stream, rules::Players_sptr players);
        GameState(const GameState& st);
        rules::GameState* copy() const override;

        int opponent(int player_id) const;

        case_type get_cell_type(position pos) const;

        void reset_pm(int player_id);
        void reset_pa(int player_id);

        bool is_finished() const;
        int get_round() const;
        void increment_round();


    private:
        std::unordered_map<int, PlayerInfo> player_info_;
        std::array<int, 2> player_ids_;

        std::shared_ptr<Map> map_;
        int round_;
};

#endif /* !GAME_STATE_HH */

