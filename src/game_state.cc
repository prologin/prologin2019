#include "game_state.hh"

GameState::GameState(std::istream& map_stream, rules::Players_sptr players)
    : rules::GameState()
    , map_(new Map(map_stream))
    , round_(0)
{
    int id = 0;
    for (auto& player : players->players)
    {
        if (id > 2)
            FATAL("This game does not support more than two players.");
        if (player->type == rules::PLAYER)
        {
            player_info_.emplace(std::make_pair(player->id,
                                                PlayerInfo(player, id)));
            player_ids_[id] = player->id;
            ++id;
        }
    }
    for (int player = 0; player < 2; ++player)
        for (int nain = 0; nain < NB_NAINS; ++nain)
        {
            nains_[player][nain].pos = map_->get_spawn_point(player);
            nains_[player][nain].vie = VIE_NAIN;
        }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(new Map(*st.map_.get()))
    , player_info_(st.player_info_)
    , player_ids_(st.player_ids_)
    , round_(st.round_)
    , nains_(st.nains_)
{}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

int GameState::opponent(int player_id) const
{
    return (player_ids_[0] == player_id) ? player_ids_[1] : player_ids_[0];
}

case_type GameState::get_cell_type(position pos) const
{
    return map_->get_cell_type(pos);
}

minerai GameState::get_minerai(position pos) const
{
    return map_->get_minerai(pos);
}

nain GameState::get_nain(int player_id, int nain_id) const
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    return nains_[internal_player_id][nain_id];
}

void GameState::reset_pm(int player_id)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[internal_player_id][nain].pm = NB_POINTS_DEPLACEMENT;
}

void GameState::reset_pa(int player_id)
{
    int internal_player_id = player_info_.at(player_id).get_internal_id();
    for (int nain = 0; nain < NB_NAINS; ++nain)
        nains_[internal_player_id][nain].pa = NB_POINTS_ACTION;
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
