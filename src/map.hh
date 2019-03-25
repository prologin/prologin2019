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

#include "constant.hh"

class Map
{
public:
    Map(std::istream& stream);
    Map(const Map& map);

    case_type get_cell_type(position pos) const;
    std::vector<position> get_ropes() const;
    bool is_rope(position pos) const;
    minerai get_minerrai(position pos) const;

    void set_cell_type(position pos, case_type type);

private:
    void load_map_cells(std::istream& stream);
    void load_minerai_info(std::istream& stream);

    std::array<std::array<case_type, TAILLE_MINE>, TAILLE_MINE> map_;

    std::array<std::array<bool, TAILLE_MINE>, TAILLE_MINE> rope_;
    std::vector<position> ropes_;

    std::array<std::array<minerai*, TAILLE_MINE>, TAILLE_MINE> ore_;
    std::vector<minerai> ores_;
};

#endif
