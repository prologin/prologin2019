// SPDX-License-Identifier: GPL-2.0-or-later
/*
** This file is part of Prologin2019, a rules library for stechec2.
**
** Copyright (c) 2019 Association Prologin <info@prologin.org>
** Copyright (c) 2019 Antoine Martin
** Copyright (c) 2019 Paul Guénézan
**
** Prologin2018 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Prologin2019.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLAYER_INFO_HH
#define PLAYER_INFO_HH

#include <rules/player.hh>

#include <string>
#include <vector>

#include "constant.hh"
#include "history.hh"

class PlayerInfo
{
public:
    PlayerInfo(std::shared_ptr<rules::Player> player);

    // Id
    int get_key() const;

    // Score
    int get_score() const;
    void increase_score(int delta);

    // Name
    const std::string& get_name() const;
    void set_name(const std::string& name);

    // History
    const std::vector<internal_action>& get_internal_history() const;
    void reset_internal_history();
    void add_internal_action(internal_action action);
    void sync_score();

private:
    std::shared_ptr<rules::Player> player_;
    std::vector<internal_action> internal_hist_;
    int score;
};

#endif
