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

#include "map.hh"
#include "player_info.hh"

class GameState : public rules::GameState
{
public:
    GameState(std::istream& map_stream, rules::Players_sptr players);
    GameState(const GameState& st);
    rules::GameState* copy() const override;

    case_type get_cell_type(position pos) const;
    std::vector<position> get_ropes() const;
    bool is_rope(position pos) const;
    minerai get_minerrai(position pos) const;
    void set_cell_type(position pos, case_type type);

    const auto& get_player_info() const { return player_info_; };

    const nain& get_nain(int player_id, int nain_id) const;
    void set_nain_position(int player_id, int nain_id, position pos);
    void reduce_pm(int player_id, int nain_id, int pm);
    void reduce_pa(int player_id, int nain_id, int pa);
    void reset_pm(unsigned int player_id);
    void reset_pa(unsigned int player_id);

    int opponent(int player) const;

    int get_score(int player_id) const;
    void increase_score(int player_id, int delta);

    bool is_finished() const;
    int get_round() const;
    void increment_round();

private:
    std::unordered_map<int, PlayerInfo> player_info_;
    std::array<int, 2> player_ids_;

    std::shared_ptr<Map> map_;
    std::array<std::array<nain, NB_NAINS>, 2> nains_;
    int round_;
};

#endif /* !GAME_STATE_HH */

