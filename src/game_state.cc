#include "game_state.hh"

#include <algorithm>
#include <cmath>

#include "position.hh"

GameState::GameState(std::istream& map_stream, rules::Players_sptr players)
    : rules::GameState()
    , player_info_(
          {PlayerInfo(players->players[0]), PlayerInfo(players->players[1])})
    , map_(map_stream)
    , round_(0)
    , init_(false)
{
    if (players->players.size() > NB_JOUEURS)
        FATAL("This game does not support more than two players.");

    for (int player = 0; player < NB_JOUEURS; ++player)
    {
        for (int nain = 0; nain < NB_NAINS; ++nain)
        {
            nains_[player][nain] = {
                map_.get_spawn_point(player), VIE_NAIN, NB_POINTS_ACTION,
                NB_POINTS_DEPLACEMENT,        false,    0};
            map_.add_nain(nain, nains_[player][nain].pos, player);
        }

        check_nain_gravity(map_.get_spawn_point(player), player);
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , player_info_(st.player_info_)
    , map_(st.map_)
    , nains_(st.nains_)
    , nains_respawn_(st.nains_respawn_)
    , round_(st.round_)
    , init_(st.init_)
{
}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

int GameState::get_player_id(int player_key) const
{
    for (int player_id = 0; player_id < 2; player_id++)
        if (get_player_key(player_id) == player_key)
            return player_id;

    return -1;
}

int GameState::get_player_key(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    return player_info_[player_id].get_key();
}

int GameState::get_opponent_id(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    return 1 - player_id;
}

Map& GameState::map()
{
    return map_;
}

const Map& GameState::map() const
{
    return map_;
}

void GameState::set_cell_type(position pos, case_type type, int current_player)
{
    assert(inside_map(pos));
    assert(current_player == 0 || current_player == 1);

    map_.set_cell_type(pos, type);

    if (type == LIBRE)
    {
        const position up = get_position_offset(pos, HAUT);
        check_rope_gravity(up, current_player);
        check_nain_gravity(up, current_player);
    }
}

bool GameState::mine_minerai(position pos, int player_id, int nain_id)
{
    assert(inside_map(pos) && map_.has_minerai_at(pos) &&
           map_.get_cell_type(pos) == GRANITE);
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);

    const minerai minerai = map_.get_minerai_at(pos);

    if (minerai.resistance > 1)
    {
        map_.set_minerai_resistance(pos, minerai.resistance - 1);
        return false;
    }

    // The mineral broke
    loot(player_id, nain_id, minerai.rendement);

    internal_action action;
    action.type = 6;
    action.fall = {-1, -1, pos};
    add_to_internal_history(player_id, action);

    map_.remove_minerai(pos);
    set_cell_type(pos, LIBRE, player_id);
    return true;
}

const nain& GameState::get_nain(int player_id, int nain_id) const
{
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(player_id == 0 || player_id == 1);
    return nains_[player_id][nain_id];
}

void GameState::set_nain_position(int player_id, int nain_id, position dest)
{
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(inside_map(dest));

    position from = nains_[player_id][nain_id].pos;

    nains_[player_id][nain_id].pos = dest;
    map_.move_nain(nain_id, from, dest);

    if (map_.get_spawn_point(player_id) == dest)
    {
        increase_score(player_id, nains_[player_id][nain_id].butin);
        nains_[player_id][nain_id].butin = 0;
        nains_[player_id][nain_id].vie = VIE_NAIN;
    }
}

int GameState::get_movement_cost(int player_id, int nain_id,
                                 direction dir) const
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS &&
           nains_[player_id][nain_id].vie > 0);
    assert(0 <= dir && dir < 4);

    const nain& nain = nains_[player_id][nain_id];
    const position dest = get_position_offset(nain.pos, dir);

    assert(inside_map(dest) && map_.get_cell_type(dest) == LIBRE);
    assert(map_.get_cell_occupant(dest) == -1 ||
           map_.get_cell_occupant(dest) == player_id);

    if (nain.accroche)
    {
        if (map_.has_rope_at(dest))
            return COUT_ESCALADER_CORDE;
        else
            return COUT_ESCALADER;
    }

    return COUT_DEPLACEMENT;
}

void GameState::set_nain_accroche(int player_id, int nain_id, bool accroche)
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);
    nains_[player_id][nain_id].accroche = accroche;

    if (!accroche)
        check_nain_gravity(nains_[player_id][nain_id].pos, player_id);
}

int GameState::get_fall_distance(int player_id, int nain_id) const
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);

    if (nains_[player_id][nain_id].accroche)
        return 0;

    position pos = nains_[player_id][nain_id].pos;
    int fall = 0;

    for (;;)
    {
        position current = get_position_offset(pos, BAS);

        if (!inside_map(current) || map_.get_cell_type(current) != LIBRE)
            break;

        if (map_.get_cell_occupant(current) == 1 - player_id)
            break;

        pos = current;
        ++fall;
    }

    return fall;
}

void GameState::check_nain_gravity(position pos, int current_player)
{
    assert(current_player == 0 || current_player == 1);

    while (inside_map(pos))
    {
        position dest = get_position_offset(pos, BAS);

        if (!inside_map(dest) || map_.get_cell_type(dest) != LIBRE)
            return;

        // Apply falling to each dwarf on current cell
        const int player_id = map_.get_cell_occupant(pos);

        if (player_id == -1)
            return;

        const std::vector<int> nains_ids = map_.get_nains_ids_at(pos);

        for (const int nain_id : nains_ids)
        {
            int fall = get_fall_distance(player_id, nain_id);

            if (fall == 0)
                continue;

            dest = pos + (BAS * fall);

            internal_action action;
            action.type = 2;
            action.fall = {player_id, nain_id, dest};
            add_to_internal_history(current_player, action);

            // Kill nain if on the opponent spawn
            if (dest == this->map().get_spawn_point(1 - player_id))
            {
                const nain nain = this->get_nain(player_id, nain_id);
                if (nain.butin > 0)
                    this->increase_score(1 - player_id, nain.butin);
                this->reduce_pv(player_id, nain_id, VIE_NAIN, player_id);
                continue;
            }

            set_nain_position(player_id, nain_id, dest);

            if (fall >= 4)
                reduce_pv(player_id, nain_id, std::pow(2, fall - 4),
                          current_player);
        }

        pos = get_position_offset(pos, HAUT);
    }
}

void GameState::reduce_pm(int player_id, int nain_id, int pm)
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(nains_[player_id][nain_id].pm >= pm);

    nains_[player_id][nain_id].pm -= pm;
}

void GameState::reset_pm(int player_id)
{
    assert(player_id == 0 || player_id == 1);

    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[player_id][nain].pm = NB_POINTS_DEPLACEMENT;
}

void GameState::reduce_pa(int player_id, int nain_id, int pa)
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(nains_[player_id][nain_id].pa >= pa);

    nains_[player_id][nain_id].pa -= pa;
}

void GameState::reset_pa(int player_id)
{
    assert(player_id == 0 || player_id == 1);

    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[player_id][nain].pa = NB_POINTS_ACTION;
}

void GameState::reduce_pv(int player_id, int nain_id, int damage,
                          int current_player)
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(current_player == 0 || current_player == 1);

    nain& nain = nains_[player_id][nain_id];
    nain.vie -= damage;

    if (nain.vie <= 0)
    {
        nain.vie = 0;
        map_.remove_nain(nain_id, nain.pos);
        nains_respawn_.push_back({player_id, nain_id});

        internal_action action;
        action.type = 3;
        action.fall = {player_id, nain_id, nain.pos};
        add_to_internal_history(current_player, action);

        check_nain_gravity(get_position_offset(nain.pos, HAUT), current_player);
    }
}

void GameState::loot(int player_id, int nain_id, int value)
{
    assert(player_id == 0 || player_id == 1);
    assert(0 <= nain_id && nain_id < NB_NAINS);

    nains_[player_id][nain_id].butin =
        std::min(nains_[player_id][nain_id].butin + value, BUTIN_MAX);
}

void GameState::respawn(int player_id)
{
    assert(player_id == 0 || player_id == 1);
    bool spawn = false;

    for (auto nain : nains_respawn_)
        if (nain.first == player_id)
        {
            const position target_pos = map_.get_spawn_point(nain.first);

            internal_action action;
            action.type = 5;
            action.fall = {
                player_id,
                nain.second,
                target_pos,
            };
            add_to_internal_history(player_id, action);

            nains_[nain.first][nain.second] = {
                target_pos /* pos */,
                VIE_NAIN /* vie */,
                NB_POINTS_ACTION /* pa */,
                NB_POINTS_DEPLACEMENT /* pm */,
                false /* accroche */,
                0 /* butin */};
            map_.add_nain(nain.second, nains_[nain.first][nain.second].pos,
                          nain.first);

            spawn = true;
        }

    if (spawn)
        check_nain_gravity(map_.get_spawn_point(player_id), player_id);

    nains_respawn_.erase(
        std::remove_if(nains_respawn_.begin(), nains_respawn_.end(),
                       [&](auto nain) { return nain.first == player_id; }),
        nains_respawn_.end());
}

void GameState::check_rope_gravity(position pos, int current_player)
{
    assert(current_player == 0 || current_player == 1);

    if (!inside_map(pos) || !map_.has_rope_at(pos))
        return;

    while (true)
    {
        if (!map_.try_extend_rope(pos))
            break;

        const Rope& rope = map_.get_rope_at(pos);

        internal_action action;
        action.type = 4;
        action.fall = {current_player, -1, rope.get_bottom()};
        add_to_internal_history(current_player, action);
    }
}

void GameState::add_rope(position pos, int current_player)
{
    assert(inside_map(pos));
    assert(current_player == 0 || current_player == 1);

    map_.add_rope(pos);
    check_rope_gravity(pos, current_player);
}

int GameState::get_score(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    return player_info_[player_id].get_score();
}

bool GameState::is_finished() const
{
    return round_ >= NB_TOURS;
}

int GameState::get_round() const
{
    return round_;
}

void GameState::increment_round()
{
    ++round_;
}

const std::array<PlayerInfo, 2>& GameState::get_player_info() const
{
    return player_info_;
}

const std::vector<internal_action>&
GameState::get_internal_history(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    return player_info_[player_id].get_internal_history();
}

const std::vector<action_hist> GameState::get_history(int player_id) const
{
    assert(player_id == 0 || player_id == 1);

    std::vector<action_hist> hist;
    std::vector<internal_action> internal_hist =
        get_internal_history(player_id);

    for (auto action : internal_hist)
        if (action.type == 1)
            hist.push_back(action.action);

    return hist;
}

void GameState::reset_internal_history(int player_id)
{
    assert(player_id == 0 || player_id == 1);
    player_info_[player_id].reset_internal_history();
}

void GameState::add_to_internal_history(int player_id, internal_action action)
{
    assert(player_id == 0 || player_id == 1);
    player_info_[player_id].add_internal_action(action);
}

void GameState::increase_score(int player_id, int delta)
{
    assert(player_id == 0 || player_id == 1);
    player_info_[player_id].increase_score(delta);
}

bool GameState::is_init() const
{
    return init_;
}

void GameState::set_init(bool init)
{
    init_ = init;
}
