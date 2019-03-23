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

#ifndef GAME_STATE_HH
#define GAME_STATE_HH

#include <array>
#include <unordered_map>

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "player_info.hh"

class GameState : public rules::GameState
{
public:
    // FIXME
    // additional parameters? for instance map
    GameState(rules::Players_sptr players);
    GameState(const GameState& st);
    rules::GameState* copy() const override;

    const auto& get_player_info() const { return player_info_; };

    int opponent(int player) const;

    int get_score(int player_id) const;
    void increase_score(int player_id, int delta);

private:
    std::unordered_map<int, PlayerInfo> player_info_;
    std::array<int, 2> player_ids_;
};

#endif /* !GAME_STATE_HH */

