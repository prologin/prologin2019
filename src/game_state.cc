#include "game_state.hh"

#include <algorithm>
#include <cmath>

#include "position.hh"

GameState::GameState(std::istream& map_stream, rules::Players_sptr players)
    : rules::GameState()
    , map_(map_stream)
    , round_(0)
{
    int id = 0;
    for (auto& player : players->players)
    {
        if (id > NB_JOUEURS)
            FATAL("This game does not support more than two players.");
        if (player->type == rules::PLAYER)
        {
            player_info_.emplace(std::make_pair(player->id,
                                                PlayerInfo(player, id)));
            player_ids_[id] = player->id;
            ++id;
        }
    }
    for (int player = 0; player < NB_JOUEURS; ++player)
    {
        for (int nain = 0; nain < NB_NAINS; ++nain)
        {
            nains_[player][nain] = { map_.get_spawn_point(player), VIE_NAIN,
                                     NB_POINTS_ACTION, NB_POINTS_DEPLACEMENT,
                                     false, 0 };
            map_.add_nain(nain, nains_[player][nain].pos, player);
            if (map_.get_rope(map_.get_spawn_point(player)) != nullptr)
                map_.add_nain_to_rope(map_.get_spawn_point(player), player, nain);
        }
        check_gravity(map_.get_spawn_point(player));
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , player_info_(st.player_info_)
    , player_ids_(st.player_ids_)
    , map_(st.map_)
    , nains_(st.nains_)
    , nains_respawn_(st.nains_respawn_)
    , round_(st.round_)
{ }

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

int GameState::opponent(int player_id) const
{
    return (player_ids_[0] == player_id) ? player_ids_[1] : player_ids_[0];
}

int GameState::internal_to_external_id(int internal_id) const
{
    for (auto& player : player_info_)
        if (player.second.get_internal_id() == internal_id)
            return player.first;
    return -1;
}

case_type GameState::get_cell_type(position pos) const
{
    return map_.get_cell_type(pos);
}

position GameState::get_spawn_point(int player_id) const
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    return map_.get_spawn_point(internal_player_id);
}

const minerai* GameState::get_minerai(position pos) const
{
    return map_.get_minerai(pos);
}

const std::vector<position>& GameState::get_ores() const
{
    return map_.get_ores();
}

void GameState::set_cell_type(position pos, case_type type)
{
    map_.set_cell_type(pos, type);
    position up = get_position_offset(pos, HAUT);
    if (inside_map(up) && type == LIBRE)
    {
        if (get_rope(up) != nullptr)
            map_.check_gravity(up);
        if (map_.get_nains_at(up).first != -1)
            check_gravity(up);
    }
}

bool GameState::mine_minerai(position pos, int player_id, int nain_id)
{
    const minerai* minerai = map_.get_minerai(pos);
    if (minerai->resistance > 1)
    {
        map_.set_minerai_resistance(pos, minerai->resistance - 1);
        return false;
    }
    nains_[player_id][nain_id].butin += map_.get_minerai(pos)->rendement;
    map_.remove_minerai(pos);
    return true;
}

const nain* GameState::get_nain(int player_id, int nain_id) const
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    return get_nain_internal(internal_player_id, nain_id);
}

const nain* GameState::get_nain_internal(int internal_player_id, int nain_id) const
{
    nain nain = nains_[internal_player_id][nain_id];
    if (nain.vie <= 0)
        return nullptr;
    return &nains_[internal_player_id][nain_id];
}

const std::pair<int, std::unordered_set<int>>& GameState::get_nains_at(position pos) const
{
    return map_.get_nains_at(pos);
}

int GameState::get_internal_cell_ownership(position pos) const
{
    const auto& nains = map_.get_nains_at(pos);
    return nains.second.empty() ? -1 : nains.first;
}

int GameState::get_cell_ownership(position pos) const
{
    int ownership = get_internal_cell_ownership(pos);
    if (ownership == -1)
        return -1;
    return internal_to_external_id(ownership);
}

void GameState::set_nain_position_internal(int player_id, int nain_id, position dest)
{
    position from = nains_[player_id][nain_id].pos;
    if (map_.get_rope(from) != nullptr)
        map_.remove_nain_from_rope(from, player_id, nain_id);
    nains_[player_id][nain_id].pos = dest;
    map_.move_nain(nain_id, from, dest);
    if (map_.get_rope(dest) != nullptr)
        map_.add_nain_to_rope(dest, player_id, nain_id);
    if (map_.get_spawn_point(player_id) == dest)
    {
        increase_score(player_id, nains_[player_id][nain_id].butin);
        nains_[player_id][nain_id].butin = 0;
        nains_[player_id][nain_id].vie = VIE_NAIN;
    }
}

void GameState::set_nain_position(int player_id, int nain_id, position dest)
{
    set_nain_position_internal(player_info_.at(player_id).get_internal_id(),
                               nain_id, dest);
}

int GameState::get_movement_cost(int player_id, int nain_id, direction dir) const
{
    if (nain_id < 0 || nain_id >= NB_NAINS)
        return -1;
    if (dir < 0 || dir >= 4)
        return -1;

    const nain* nain = get_nain(player_id, nain_id);
    if (nain == nullptr)
        return -1;

    position dest = get_position_offset(nain->pos, dir);
    if (!inside_map(dest))
        return -1;
    if (get_cell_type(dest) != LIBRE)
        return -1;
    int dest_owner = get_cell_ownership(dest);
    if (dest_owner != -1 && dest_owner != player_id)
        return -1;

    if (nain->accroche)
        return COUT_ESCALADER;
    return COUT_DEPLACEMENT;
}

void GameState::set_nain_accroche(int player_id, int nain_id, bool accroche)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    nains_[internal_player_id][nain_id].accroche = accroche;
    if (!accroche)
        check_gravity(nains_[internal_player_id][nain_id].pos);
}

int GameState::get_fall_distance(int player_id, int nain_id) const
{
    if (player_id == -1 || nains_[player_id][nain_id].accroche)
        return 0;
    position pos = nains_[player_id][nain_id].pos;
    int fall = 0;
    for (;;)
    {
        position current = get_position_offset(pos, BAS);
        if (!inside_map(current) || map_.get_cell_type(current) != LIBRE)
            break;
        if (get_cell_ownership(current) == player_id)
            break;
        pos = current;
        ++fall;
    }
    return fall;
}

void GameState::check_gravity(position pos)
{
    if (get_rope(pos) != nullptr)
        return;
    position dest = get_position_offset(pos, BAS);
    if (!inside_map(dest) || get_cell_type(dest) != LIBRE)
        return;
    const auto& nains = map_.get_nains_at(pos);
    int player_id = nains.first;
    std::unordered_set<int> id_nains(nains.second);
    for (int nain_id : std::unordered_set<int>(nains.second))
    {
        int fall = get_fall_distance(player_id, nain_id);
        if (fall == 0)
            continue;
        set_nain_position_internal(player_id, nain_id, pos + (BAS * fall));
        if (fall < 4)
            continue;
        reduce_pv_internal(player_id, nain_id, std::pow(2, fall - 4));
    }
    position up = get_position_offset(pos, HAUT);
    if (inside_map(up))
        check_gravity(up);
}

void GameState::reduce_pm(int player_id, int nain_id, int pm)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    nains_[internal_player_id][nain_id].pm -= pm;
}

void GameState::reset_pm(int player_id)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[internal_player_id][nain].pm = NB_POINTS_DEPLACEMENT;
}

void GameState::reduce_pa(int player_id, int nain_id, int pa)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    nains_[internal_player_id][nain_id].pa -= pa;
}

void GameState::reset_pa(int player_id)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[internal_player_id][nain].pa = NB_POINTS_ACTION;
}

void GameState::reduce_pv(int player_id, int nain_id, int damage)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    reduce_pv_internal(internal_player_id, nain_id, damage);
}

void GameState::reduce_pv_internal(int internal_player_id, int nain_id, int damage)
{
    nains_[internal_player_id][nain_id].vie -= damage;
    if (nains_[internal_player_id][nain_id].vie <= 0)
    {
        nains_[internal_player_id][nain_id].vie = 0;
        map_.remove_nain(nain_id, nains_[internal_player_id][nain_id].pos);
        nains_respawn_.push_back({ internal_player_id, nain_id });
    }
}

void GameState::respawn(int player_id)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    bool spawn = false;
    for (auto nain : nains_respawn_)
        if (nain.first == internal_player_id)
        {
            nains_[nain.first][nain.second] = { map_.get_spawn_point(nain.first),
                                               VIE_NAIN, NB_POINTS_DEPLACEMENT,
                                               NB_POINTS_ACTION, false, 0 };
            map_.add_nain(nain.second, nains_[nain.first][nain.second].pos,
                          nain.first);
            if (map_.get_rope(map_.get_spawn_point(nain.first)) != nullptr)
                map_.add_nain_to_rope(map_.get_spawn_point(nain.first), nain.first, nain.second);
            spawn = true;
        }
    if (spawn)
        check_gravity(map_.get_spawn_point(internal_player_id));
    nains_respawn_.erase(std::remove_if(nains_respawn_.begin(), nains_respawn_.end(),
                   [&] (auto nain) { return nain.first == internal_player_id; }), nains_respawn_.end());
}

const std::vector<position>& GameState::get_ropes() const
{
    return map_.get_ropes();
}

const std::vector<Rope> GameState::get_base_ropes() const
{
    return map_.get_base_ropes();
}

const Rope* GameState::get_rope(position pos) const
{
    return map_.get_rope(pos);
}

void GameState::add_rope(position pos)
{
    map_.add_rope(pos);
}

int GameState::get_score(int player_id) const
{
    assert(player_info_.count(player_id) != 0);
    return player_info_.at(player_id).get_score();
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

const std::vector<internal_action>& GameState::get_internal_history(int player_id) const
{
    assert(player_info_.count(player_id) != 0);
    return player_info_.at(player_id).get_internal_history();
}

const std::vector<action_hist> GameState::get_history(int player_id) const
{
    std::vector<internal_action> internal_hist = get_internal_history(player_id);
    std::vector<action_hist> hist;
    for (auto action : internal_hist)
        if (!action.internal)
            hist.push_back(action.action);
    return hist;
}

void GameState::reset_internal_history(int player_id)
{
    assert(player_info_.count(player_id) != 0);
    player_info_.at(player_id).reset_internal_history();
}

void GameState::add_to_internal_history(int player_id, internal_action action)
{
    assert(player_info_.count(player_id) != 0);
    player_info_.at(player_id).add_internal_action(action);
}

void GameState::increase_score(int player_id, int delta)
{
    assert(player_info_.count(player_id) != 0);
    player_info_.at(player_id).increase_score(delta);
}
