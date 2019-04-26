// SPDX-License-Identifier: GPL-2.0-or-later
/*
** This file is part of Prologin2019, a rules library for stechec2.
**
** Copyright (c) 2019 Association Prologin <info@prologin.org>
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

#ifndef MAP_HH
#define MAP_HH

#include <array>
#include <istream>
#include <vector>
#include <unordered_set>

#include "constant.hh"
#include "rope.hh"

class Map
{
public:
    Map(std::istream& stream);
    Map(const Map& map);

    position get_spawn_point(int player_id) const;

    case_type get_cell_type(position pos) const;
    const std::vector<position>& get_ropes() const;
    const Rope* get_rope(position pos) const;
    minerai get_minerai(position pos) const;

    void set_cell_type(position pos, case_type type);
    void add_rope(Rope& rope);
    void extends_rope(position pos);

    void add_nain(int nain_id, position pos, int player_id);
    void move_nain(int nain_id, position from, position to);
    const std::pair<int, std::unordered_set<int>>& get_nains_at(position pos) const;

private:
    void load_map_cells(std::istream& stream);
    void load_spawn_point(std::istream& stream);
    void load_minerai_info(std::istream& stream);

    std::array<std::array<std::pair<int, std::unordered_set<int>>, TAILLE_MINE>,
               TAILLE_MINE> nains_;

    std::array<std::array<case_type, TAILLE_MINE>, TAILLE_MINE> map_;
    std::array<position, 2> spawn_point_;

    std::array<std::array<Rope*, TAILLE_MINE>, TAILLE_MINE> rope_;
    std::vector<Rope> ropes_;
    std::vector<position> ropes_pos_;

    std::vector<minerai> ores_;
    std::array<std::array<minerai*, TAILLE_MINE>, TAILLE_MINE> ore_;
};

#endif
