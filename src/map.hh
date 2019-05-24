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

#pragma once

#include <array>
#include <istream>
#include <vector>

#include "constant.hh"
#include "rope.hh"

typedef struct Cell
{
    case_type type;
    int rope;     // -1 if there is none
    minerai ore;  // attributes set to -1 if there is no mineral
    int occupant; // -1 when there isn't any dwarf
    std::vector<int> nains_ids;
} Cell;

class Map
{
public:
    Map(std::istream& stream);
    Map(const Map& map);

    position get_spawn_point(int player_id) const;

    // Cells
    case_type get_cell_type(position pos) const;
    const minerai* get_minerai(position pos) const;
    const std::vector<position>& get_ores() const;

    void set_cell_type(position pos, case_type type);
    void remove_minerai(position pos);
    void set_minerai_resistance(position pos, int resistance);

    // Nains
    void add_nain(int nain_id, position pos, int player_id);
    void move_nain(int nain_id, position from, position to);
    void remove_nain(int nain_id, position pos);
    const std::vector<int>& get_nains_ids_at(position pos) const;
    int get_cell_occupant(position pos) const;

    // Ropes
    void add_rope(position pos);
    bool has_rope_at(position pos) const;
    const Rope& get_rope_at(position pos) const;
    void add_nain_to_rope(position pos, int player_id, int nain_id);
    void remove_nain_from_rope(position pos, int player_id, int nain_id);
    const std::vector<Rope> get_base_ropes() const;
    std::vector<position> get_ropes_positions() const;

    /// Try to expend the rope 1 cell towards the bottom direction, return true
    /// when a new rope cell is created at the bottom of the rope.
    bool try_extend_rope(position pos);

    std::vector<direction> get_shortest_path(position start,
                                             position dest) const;

private:
    void load_map_cells(std::istream& stream);
    void load_spawn_point(std::istream& stream);
    void load_minerai_info(std::istream& stream);
    void load_rope_info(std::istream& stream);

    std::array<std::array<Cell, TAILLE_MINE>, TAILLE_MINE> map_;
    std::array<position, 2> spawn_point_;
    std::vector<position> ores_pos_;
    std::vector<Rope> ropes_;
};
