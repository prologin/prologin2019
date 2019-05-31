// FIXME License notice

#ifndef GAME_STATE_HH
#define GAME_STATE_HH

#include <unordered_map>
#include <vector>

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

    // Player ids manipulation
    int get_player_id(int player_key) const;
    int get_player_key(int player_id) const;
    int get_opponent_id(int player_id) const;

    // Map manipulation
    Map& map();
    const Map& map() const;

    void set_cell_type(position pos, case_type type, int current_player);
    bool mine_minerai(position pos, int player_id, int nain_id);
    const nain& get_nain(int player_id, int nain_id) const;

    // Dwarfs movements
    void set_nain_position(int player_id, int nain_id, position dest);
    int get_movement_cost(int player_id, int nain_id, direction dir) const;
    void set_nain_accroche(int player_id, int nain_id, bool accroche);

    void check_nain_gravity(position pos, int current_player);

    void check_rope_gravity(position pos, int current_player);

    // Nain's ressources
    void reduce_pm(int player_id, int nain_id, int pm);
    void reset_pm(int player_id);
    void reduce_pa(int player_id, int nain_id, int pa);
    void reset_pa(int player_id);
    void reduce_pv(int player_id, int nain_id, int damage, int current_player);
    void loot(int player_id, int nain_id, int value);
    void respawn(int player_id);

    // Ropes
    void add_rope(position pos, int current_player);

    // Score
    int get_score(int player_id) const;
    void increase_score(int player_id, int delta);

    bool is_finished() const;
    int get_round() const;
    void increment_round();

    // NOTE: usefull for the dumper
    const std::array<PlayerInfo, 2>& get_player_info() const;

    // History
    const std::vector<internal_action>&
    get_internal_history(int player_id) const;
    const std::vector<action_hist> get_history(int player_id) const;
    void reset_internal_history(int player_id);
    void add_to_internal_history(int player_id, internal_action action);

private:
    int get_fall_distance(int player_id, int nain_id) const;

    std::array<PlayerInfo, 2> player_info_;

    Map map_;
    std::array<std::array<nain, NB_NAINS>, 2> nains_;
    std::vector<std::pair<int, int>> nains_respawn_;
    int round_;
};

#endif /* !GAME_STATE_HH */
