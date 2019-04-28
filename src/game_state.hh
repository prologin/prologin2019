// FIXME License notice

#ifndef GAME_STATE_HH
#define GAME_STATE_HH

#include <vector>
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
        int internal_to_external_id(int internal_id) const;

        case_type get_cell_type(position pos) const;
        position get_spawn_point(int player_id) const;
        const minerai* get_minerai(position pos) const;
        const std::vector<position>& get_ores() const;
        void set_cell_type(position pos, case_type type);
        bool mine_minerai(position pos, int player_id, int nain_id);

        const nain* get_nain(int player_id, int nain_id) const;
        const std::pair<int, std::unordered_set<int>>& get_nains_at(position pos) const;
        int get_cell_ownership(position pos) const;
        int get_internal_cell_ownership(position pos) const;

        void set_nain_position(int player_id, int nain_id, position dest);
        void set_nain_position_internal(int player_id, int nain_id, position dest);
        int get_movement_cost(int player_id, int nain_id, direction dir) const;
        void set_nain_accroche(int player_id, int nain_id, bool accroche);
        void check_gravity(position pos);

        void reduce_pm(int player_id, int nain_id, int pm);
        void reset_pm(int player_id);
        void reduce_pa(int player_id, int nain_id, int pa);
        void reset_pa(int player_id);
        void reduce_pv(int player_id, int nain_id, int damage);
        void respawn(int player_id);

        const std::vector<position>& get_ropes() const;
        const Rope* get_rope(position pos) const;
        void add_rope(position pos);

        int get_score(int player_id) const;
        bool is_finished() const;
        int get_round() const;
        void increment_round();

        const auto& get_player_info() const { return player_info_; };

        const std::vector<internal_action>& get_internal_history(int player_id) const;
        const std::vector<action_hist> get_history(int player_id) const;
        void reset_internal_history(int player_id);
        void add_to_internal_history(int player_id, internal_action action);

    private:
        int get_fall_distance(int player_id, int nain_id) const;
        void increase_score(int player_id, int delta);

        std::unordered_map<int, PlayerInfo> player_info_;
        std::array<int, 2> player_ids_;

        Map map_;
        std::array<std::array<nain, NB_NAINS>, 2> nains_;
        std::vector<std::pair<int, int>> nains_respawn_;
        int round_;
};

#endif /* !GAME_STATE_HH */

