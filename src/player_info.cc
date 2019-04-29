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

#include "player_info.hh"

PlayerInfo::PlayerInfo(rules::Player_sptr player, int internal_id)
    : player_(std::move(player))
    , internal_id_(internal_id)
{
    assert(player_);
    player_->score = 0;
}

int PlayerInfo::get_internal_id() const
{
    return internal_id_;
}

int PlayerInfo::get_score() const
{
    return player_->score;
}

void PlayerInfo::increase_score(int delta)
{
    assert(true == false);
    player_->score += delta;
}

const std::string& PlayerInfo::get_name() const
{
    return player_->name;
}

void PlayerInfo::set_name(const std::string& name)
{
    player_->name = name;
}

const std::vector<internal_action>& PlayerInfo::get_internal_history() const
{
    return internal_hist_;
}

void PlayerInfo::reset_internal_history()
{
    internal_hist_.clear();
}

void PlayerInfo::add_internal_action(internal_action action)
{
    internal_hist_.push_back(action);
}
