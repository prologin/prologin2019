// SPDX-License-Identifier: GPL-2.0-or-later
/*
** This file is part of Prologin2019, a rules library for stechec2.
**
** Copyright (c) 2019 Association Prologin <info@prologin.org>
** Copyright (c) 2019 Antoine Martin
**
** Prologin2019 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Prologin2019.  If not, see <http://www.gnu.org/licenses/>.
*/

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
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(new Map(*st.map_.get()))
    , player_info_(st.player_info_)
    , player_ids_(st.player_ids_)
    , round_(st.round_)
{}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

case_type GameState::get_cell_type(position pos) const
{
    return map_->get_cell_type(pos);
}

NainInfo GameState::get_nain_info(int player_id, int nain_id) const
{
    return nain_info_[player_id][nain_id];
}

void GameState::set_nain_position(int player_id, int nain_id, position pos)
{
    nain_info_[player_id][nain_id].set_position(pos);
}

int GameState::opponent(int player) const
{
    return (player_ids_[0] == player) ? player_ids_[1] : player_ids_[0];
}

int GameState::get_score(int player_id) const
{
    auto player = player_info_.find(player_id);
    assert(player != player_info_.end());
    return player->second.get_score();
}

void GameState::increase_score(int player_id, int delta)
{
    auto player = player_info_.find(player_id);
    assert(player != player_info_.end());
    return player->second.increase_score(delta);
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
