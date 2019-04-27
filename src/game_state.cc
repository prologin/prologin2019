#include "game_state.hh"

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
        for (int nain = 0; nain < NB_NAINS; ++nain)
        {
            nains_[player][nain] = { map_.get_spawn_point(player), VIE_NAIN,
                                     NB_POINTS_DEPLACEMENT, NB_POINTS_ACTION,
                                     false, 0 };
            map_.add_nain(nain, nains_[player][nain].pos, player);
        }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(st.map_)
    , player_info_(st.player_info_)
    , player_ids_(st.player_ids_)
    , round_(st.round_)
    , nains_(st.nains_)
    , nains_respawn_(st.nains_respawn_)
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

const minerai* GameState::get_minerai(position pos) const
{
    return map_.get_minerai(pos);
}

void GameState::set_cell_type(position pos, case_type type)
{
    map_.set_cell_type(pos, type);
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
//    return ownership;
    return internal_to_external_id(ownership);
}

void GameState::set_nain_position_internal(int player_id, int nain_id, position dest)
{
    position from = nains_[player_id][nain_id].pos;
    nains_[player_id][nain_id].pos = dest;
    map_.move_nain(nain_id, from, dest);
}

void GameState::set_nain_position(int player_id, int nain_id, position dest)
{
    set_nain_position_internal(player_info_.at(player_id).get_internal_id(),
                               nain_id, dest);
}

void GameState::set_nain_accroche(int player_id, int nain_id, bool accroche)
{
    nains_[player_id][nain_id].accroche = accroche;
    check_gravity(nains_[player_id][nain_id].pos);
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
        int ownership = get_cell_ownership(current);
        if (ownership != -1 && ownership != player_id)
            break;
        pos = current;
        ++fall;
    }
    return fall;
}

void GameState::check_gravity(position pos)
{
    if (get_cell_ownership(pos) == -1)
        return;
    const auto& nains = map_.get_nains_at(pos);
    for (int nain_id : nains.second)
    {
        int fall = get_fall_distance(nains.first, nain_id);
        if (fall == 0)
            continue;
        // TODO compute fall damage
        set_nain_position_internal(nains.first, nain_id, pos + (BAS * fall));
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
    nains_[internal_player_id][nain_id].vie -= damage;
    if (nains_[internal_player_id][nain_id].vie <= 0)
    {
        nains_[internal_player_id][nain_id].vie = 0;
        map_.remove_nain(nain_id, nains_[internal_player_id][nain_id].pos);
        nains_respawn_.push_back({ player_id, nain_id });
    }
}

const std::vector<position>& GameState::get_ropes() const
{
    return map_.get_ropes();
}

const Rope* GameState::get_rope(position pos) const
{
    return map_.get_rope(pos);
}

void GameState::add_rope(Rope& rope)
{
    map_.add_rope(rope);
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
