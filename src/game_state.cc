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
{
}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

std::vector<direction> GameState::get_shortest_path(position start,
                                                    position dest) const
{
    return map_.get_shortest_path(start, dest);
}

int GameState::get_player_id(int player_key) const
{
    return (get_player_key(0) == player_key) ? 0 : 1;
}

int GameState::get_player_key(int player_id) const
{
    return player_info_[player_id].get_key();
}

int GameState::get_opponent_id(int player_id) const
{
    return 1 - player_id;
}

case_type GameState::get_cell_type(position pos) const
{
    return map_.get_cell_type(pos);
}

position GameState::get_spawn_point(int player_id) const
{
    return map_.get_spawn_point(player_id);
}

bool GameState::has_minerai_at(position pos) const
{
    return map_.has_minerai_at(pos);
}

minerai GameState::get_minerai_at(position pos) const
{
    return map_.get_minerai_at(pos);
}

const std::vector<position>& GameState::get_ores() const
{
    return map_.get_ores();
}

void GameState::set_cell_type(position pos, case_type type, int current_player)
{
    map_.set_cell_type(pos, type);
    position up = get_position_offset(pos, HAUT);

    if (type == LIBRE)
    {
        check_rope_gravity(up, current_player);
        check_nain_gravity(up, current_player);
    }
}

bool GameState::mine_minerai(position pos, int player_id, int nain_id)
{
    assert(map_.has_minerai_at(pos));
    minerai minerai = map_.get_minerai_at(pos);

    if (minerai.resistance > 1)
    {
        map_.set_minerai_resistance(pos, minerai.resistance - 1);
        return false;
    }

    nains_[player_id][nain_id].butin += minerai.rendement;
    map_.remove_minerai(pos);
    return true;
}

const nain& GameState::get_nain(int player_id, int nain_id) const
{
    return nains_[player_id][nain_id];
}

const std::vector<int>& GameState::get_nains_ids_at(position pos) const
{
    return map_.get_nains_ids_at(pos);
}

int GameState::get_cell_occupant(position pos) const
{
    return map_.get_cell_occupant(pos);
}

void GameState::set_nain_position(int player_id, int nain_id, position dest)
{
    position from = nains_[player_id][nain_id].pos;

    if (map_.has_rope_at(from))
        map_.remove_nain_from_rope(from, player_id, nain_id);

    nains_[player_id][nain_id].pos = dest;
    map_.move_nain(nain_id, from, dest);

    if (map_.get_spawn_point(player_id) == dest)
    {
        increase_score(player_id, nains_[player_id][nain_id].butin);
        nains_[player_id][nain_id].butin = 0;
        nains_[player_id][nain_id].vie = VIE_NAIN;
    }

    if (map_.has_rope_at(dest) && nains_[player_id][nain_id].accroche)
        add_nain_to_rope(dest, player_id, nain_id);
}

int GameState::get_movement_cost(int player_id, int nain_id,
                                 direction dir) const
{
    if (nain_id < 0 || nain_id >= NB_NAINS || dir < 0 || dir >= 4 ||
        nains_[player_id][nain_id].vie <= 0)
        return -1;

    const nain& nain = nains_[player_id][nain_id];
    position dest = get_position_offset(nain.pos, dir);

    if (!inside_map(dest) || get_cell_type(dest) != LIBRE)
        return -1;

    int dest_owner = get_cell_occupant(dest);

    if (dest_owner != -1 && dest_owner != player_id)
        return -1;

    if (nain.accroche)
    {

        if (has_rope_at(dest))
            return COUT_ESCALADER_CORDE;
        else
            return COUT_ESCALADER;
    }

    return COUT_DEPLACEMENT;
}

void GameState::set_nain_accroche(int player_id, int nain_id, bool accroche)
{
    nains_[player_id][nain_id].accroche = accroche;

    if (!accroche)
        check_nain_gravity(nains_[player_id][nain_id].pos, player_id);
}

int GameState::get_fall_distance(int player_id, int nain_id) const
{
    if (nains_[player_id][nain_id].accroche)
        return 0;

    position pos = nains_[player_id][nain_id].pos;
    int fall = 0;

    for (;;)
    {
        position current = get_position_offset(pos, BAS);

        if (!inside_map(current) || map_.get_cell_type(current) != LIBRE)
            break;

        if (get_cell_occupant(current) != player_id)
            break;

        pos = current;
        ++fall;
    }

    return fall;
}

void GameState::check_nain_gravity(position pos, int current_player)
{
    while (inside_map(pos))
    {
        position dest = get_position_offset(pos, BAS);

        if (!inside_map(dest) || get_cell_type(dest) != LIBRE)
            return;

        // Apply falling to each dwarf on current cell
        const int player_id = get_cell_occupant(pos);

        if (player_id == -1)
            return;

        // TODO: avoid this (heavy?) copy
        const std::vector<int> nains_ids = get_nains_ids_at(pos);
        for (const int nain_id : nains_ids)
        {
            int fall = get_fall_distance(player_id, nain_id);
            if (fall == 0)
                continue;

            set_nain_position(player_id, nain_id, pos + (BAS * fall));

            if (fall >= 4)
                reduce_pv(player_id, nain_id, std::pow(2, fall - 4),
                          current_player);

            internal_action action;
            action.type = 2;
            action.fall = {player_id, nain_id, pos + (BAS * fall)};
            add_to_internal_history(current_player, action);
        }

        pos = get_position_offset(pos, HAUT);
    }
}

void GameState::reduce_pm(int player_id, int nain_id, int pm)
{
    assert(nains_[player_id][nain_id].pm >= pm);
    nains_[player_id][nain_id].pm -= pm;
}

void GameState::reset_pm(int player_id)
{
    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[player_id][nain].pm = NB_POINTS_DEPLACEMENT;
}

void GameState::reduce_pa(int player_id, int nain_id, int pa)
{
    assert(nains_[player_id][nain_id].pa >= pa);
    nains_[player_id][nain_id].pa -= pa;
}

void GameState::reset_pa(int player_id)
{
    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[player_id][nain].pa = NB_POINTS_ACTION;
}

void GameState::reduce_pv(int player_id, int nain_id, int damage,
                          int current_player)
{
    nain& nain = nains_[player_id][nain_id];
    nain.vie -= damage;

    if (nain.vie <= 0)
    {
        if (map_.has_rope_at(nain.pos))
            remove_nain_from_rope(nain.pos, player_id, nain_id);

        nain.vie = 0;
        map_.remove_nain(nain_id, nain.pos);
        nains_respawn_.push_back({player_id, nain_id});

        internal_action action;
        action.type = 3;
        action.fall = {player_id, nain_id, nain.pos};
        add_to_internal_history(current_player, action);
    }
}

void GameState::respawn(int player_id)
{
    bool spawn = false;

    for (auto nain : nains_respawn_)
        if (nain.first == player_id)
        {
            nains_[nain.first][nain.second] = {map_.get_spawn_point(nain.first),
                                               VIE_NAIN,
                                               NB_POINTS_DEPLACEMENT,
                                               NB_POINTS_ACTION,
                                               false,
                                               0};
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

std::vector<position> GameState::get_ropes() const
{
    return map_.get_ropes_positions();
}

std::vector<Rope> GameState::get_base_ropes() const
{
    return map_.get_base_ropes();
}

void GameState::check_rope_gravity(position pos, int current_player)
{
    if (!inside_map(pos) || !has_rope_at(pos))
        return;

    while (true)
    {
        if (!map_.try_extend_rope(pos))
            break;

        const Rope& rope = get_rope_at(pos);
        update_nains_on_rope(rope.get_bottom());

        internal_action action;
        action.type = 4;
        action.fall = {current_player, -1, rope.get_bottom()};
        add_to_internal_history(current_player, action);
    }
}

void GameState::update_nains_on_rope(position pos)
{
    const int player_id = get_cell_occupant(pos);
    const auto nains = get_nains_ids_at(pos);

    for (int id_nain : nains)
        if (nains_[player_id][id_nain].accroche)
            map_.add_nain_to_rope(pos, player_id, id_nain);
}

const Rope& GameState::get_rope_at(position pos) const
{
    return map_.get_rope_at(pos);
}

bool GameState::has_rope_at(position pos) const
{
    return inside_map(pos) && map_.has_rope_at(pos);
}

void GameState::add_rope(position pos, int current_player)
{
    map_.add_rope(pos);
    update_nains_on_rope(pos);
    check_rope_gravity(pos, current_player);
}

void GameState::add_nain_to_rope(position pos, int player_id, int nain_id)
{
    map_.add_nain_to_rope(pos, player_id, nain_id);
}

void GameState::remove_nain_from_rope(position pos, int player_id, int nain_id)
{
    map_.remove_nain_from_rope(pos, player_id, nain_id);
}

int GameState::get_score(int player_id) const
{
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

const std::vector<internal_action>&
GameState::get_internal_history(int player_id) const
{
    return player_info_[player_id].get_internal_history();
}

const std::vector<action_hist> GameState::get_history(int player_id) const
{
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
    player_info_[player_id].reset_internal_history();
}

void GameState::add_to_internal_history(int player_id, internal_action action)
{
    player_info_[player_id].add_internal_action(action);
}

void GameState::increase_score(int player_id, int delta)
{
    player_info_[player_id].increase_score(delta);
}
