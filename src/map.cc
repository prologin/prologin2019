#include "map.hh"

#include <algorithm>
#include <cassert>
#include <queue>
#include <utils/log.hh>

#include "position.hh"

void Map::load_map_cells(std::istream& stream)
{
    for (int l = 0; l < TAILLE_MINE; ++l)
    {
        std::string line;
        stream >> line;
        if (line.length() != (size_t)TAILLE_MINE)
            FATAL("map: line %d is not the right length "
                  "(is %d long, should be %d)",
                  l, line.length(), TAILLE_MINE);

        for (int c = 0; c < TAILLE_MINE; ++c)
        {
            switch (line[c])
            {
            case '.':
                map_[l][c].type = LIBRE;
                break;
            case 'X':
                map_[l][c].type = GRANITE;
                break;
            case '#':
                map_[l][c].type = OBSIDIENNE;
                break;
            default:
                FATAL("Invalid cell type '%c' at (%d;%d)", line[c], l, c);
                break;
            }
        }
    }
}

void Map::load_spawn_point(std::istream& stream)
{
    for (int player = 0; player < 2; ++player)
    {
        int l, c;
        stream >> l >> c;
        spawn_point_[player] = {l, c};
        position down = get_position_offset(spawn_point_[player], BAS);

        if (inside_map(down))
            map_[down.ligne][down.colonne].type = OBSIDIENNE;
    }
}

void Map::load_minerai_info(std::istream& stream)
{
    std::vector<position> seen;
    int nb_minerai;

    stream >> nb_minerai;
    ores_pos_.resize(nb_minerai);

    for (int minerai = 0; minerai < nb_minerai; ++minerai)
    {
        int l, c;
        stream >> l >> c;
        position pos = {l, c};

        std::string error;
        if (std::find(seen.cbegin(), seen.cend(), pos) != seen.cend())
            error = "already taken";
        if (!inside_map(pos))
            error = "outside of map";
        if (map_[l][c].type != GRANITE)
            error = "not in granite";

        if (error != "")
            FATAL("invalid position (%d;%d) for minerai is %s", l, c,
                  error.c_str());

        int resistance, rendement;
        stream >> resistance >> rendement;

        if (resistance <= 0)
            FATAL("resistance must be a strictly positive int");

        map_[l][c].ore = {resistance, rendement};
        ores_pos_[minerai] = {l, c};
        seen.push_back(pos);
    }
}

void Map::load_rope_info(std::istream& stream)
{
    int nb_ropes;
    stream >> nb_ropes;

    for (int i = 0; i < nb_ropes; ++i)
    {
        int l, c;
        stream >> l >> c;
        position pos = {l, c};

        std::string error;

        if (!inside_map(pos))
            error = "outside of map";

        if (map_[l][c].type != LIBRE)
            error = "not in libre";

        if (error != "")
            FATAL("invalid position (%d;%d) for rope is %s", l, c,
                  error.c_str());

        add_rope(pos);
        while (try_extend_rope(pos))
            ;
    }
}

Map::Map(std::istream& stream)
{
    INFO("Loading map");

    for (int x = 0; x < TAILLE_MINE; ++x)
        for (int y = 0; y < TAILLE_MINE; ++y)
            map_[y][x] = {ERREUR_CASE, -1, {-1, -1}, -1, {}};

    load_map_cells(stream);
    load_spawn_point(stream);
    load_minerai_info(stream);
    load_rope_info(stream);
}

Map::Map(const Map& map)
    : map_(map.map_)
    , spawn_point_(map.spawn_point_)
    , ores_pos_(map.ores_pos_)
    , ropes_(map.ropes_)
{
}

position Map::get_spawn_point(int player_id) const
{
    return spawn_point_[player_id];
}

case_type Map::get_cell_type(position pos) const
{
    assert(inside_map(pos));
    return map_[pos.ligne][pos.colonne].type;
}

bool Map::has_minerai_at(position pos) const
{
    assert(inside_map(pos));
    return map_[pos.ligne][pos.colonne].ore.resistance != -1;
}

minerai Map::get_minerai_at(position pos) const
{
    assert(inside_map(pos));
    return map_[pos.ligne][pos.colonne].ore;
}

const std::vector<position>& Map::get_ores() const
{
    return ores_pos_;
}

void Map::set_cell_type(position pos, case_type type)
{
    assert(inside_map(pos));
    map_[pos.ligne][pos.colonne].type = type;
}

inline bool operator==(const minerai& a, const minerai& b)
{
    return a.resistance == b.resistance && a.rendement == b.rendement;
}

void Map::remove_minerai(position pos)
{
    assert(inside_map(pos));
    map_[pos.ligne][pos.colonne].ore = {-1, -1};
    ores_pos_.erase(std::remove(ores_pos_.begin(), ores_pos_.end(), pos),
                    ores_pos_.end());
}

void Map::set_minerai_resistance(position pos, int resistance)
{
    assert(inside_map(pos));
    map_[pos.ligne][pos.colonne].ore.resistance = resistance;
}

void Map::add_nain(int nain_id, position pos, int player_id)
{
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(inside_map(pos));

    Cell& cell = map_[pos.ligne][pos.colonne];
    assert(cell.occupant == -1 ||
           (!cell.nains_ids.empty() && cell.occupant == player_id));

    cell.occupant = player_id;
    cell.nains_ids.push_back(nain_id);
}

void Map::move_nain(int nain_id, position from, position to)
{
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(inside_map(from));
    assert(inside_map(to));

    const int player_id = map_[from.ligne][from.colonne].occupant;
    add_nain(nain_id, to, player_id);
    remove_nain(nain_id, from);
}

void Map::remove_nain(int nain_id, position pos)
{
    assert(0 <= nain_id && nain_id < NB_NAINS);
    assert(inside_map(pos));

    Cell& cell = map_[pos.ligne][pos.colonne];

    cell.nains_ids.erase(
        std::remove(cell.nains_ids.begin(), cell.nains_ids.end(), nain_id),
        cell.nains_ids.end());

    if (cell.nains_ids.empty())
        cell.occupant = -1;
}

const std::vector<int>& Map::get_nains_ids_at(position pos) const
{
    assert(inside_map(pos));
    return map_[pos.ligne][pos.colonne].nains_ids;
}

int Map::get_cell_occupant(position pos) const
{
    assert(inside_map(pos));
    return map_[pos.ligne][pos.colonne].occupant;
}

inline bool operator==(const Rope& a, const Rope& b)
{
    return a.get_anchor() == b.get_anchor() && a.get_bottom() == b.get_bottom();
}

void Map::add_rope(position pos)
{
    assert(inside_map(pos));
    map_[pos.ligne][pos.colonne].rope = ropes_.size();
    ropes_.push_back(Rope(pos));
}

bool Map::has_rope_at(position pos) const
{
    assert(inside_map(pos));
    return map_[pos.ligne][pos.colonne].rope != -1;
}

const Rope& Map::get_rope_at(position pos) const
{
    assert(inside_map(pos) && has_rope_at(pos));
    const int index = map_[pos.ligne][pos.colonne].rope;
    return ropes_[index];
}

std::vector<position> Map::get_ropes_positions() const
{
    std::vector<position> result;

    for (const Rope rope : ropes_)
    {
        const std::vector<position> positions = rope.get_positions();
        result.insert(result.end(), positions.begin(), positions.end());
    }

    return result;
}

const std::vector<Rope> Map::get_base_ropes() const
{
    return ropes_;
}

bool Map::try_extend_rope(position pos)
{
    assert(inside_map(pos));
    const int rope_index = map_[pos.ligne][pos.colonne].rope;

    if (rope_index == -1)
        return false;

    position dest = get_position_offset(ropes_[rope_index].get_bottom(), BAS);
    if (!inside_map(dest) || get_cell_type(dest) != LIBRE)
        return false;

    const int dest_index = map_[dest.ligne][dest.colonne].rope;

    if (dest_index == -1)
    {
        ropes_[rope_index].extends(dest);
        map_[dest.ligne][dest.colonne].rope = rope_index;
        return true;
    }

    // Merge with the bottom rope
    ropes_[rope_index].merge_down(ropes_[dest_index]);
    const auto& positions = ropes_[dest_index].get_positions();

    for (position pos : positions)
        map_[pos.ligne][pos.colonne].rope = rope_index;

    // Remove the rope from the map
    ropes_.erase(ropes_.begin() + dest_index);

    for (auto& row : map_)
        for (Cell& cell : row)
            if (cell.rope > dest_index)
                cell.rope--;

    return false;
}

std::vector<direction> Map::get_shortest_path(position start,
                                              position dest) const
{
    assert(inside_map(start));
    assert(inside_map(dest));

    if (start == dest)
        return {};

    // Keep track of the predecessor of each cell in a shortest path from start
    // to any cell.
    std::array<std::array<direction, TAILLE_MINE>, TAILLE_MINE> predecessor;

    for (auto& ligne : predecessor)
        ligne.fill(ERREUR_DIRECTION);

    // Current component to explore: the queue is ordered in increasing number
    // of moves to use, all cells in this queue are accessible with the same
    // minimal number of mined blocks (initialy 0).
    std::queue<position> current_component;
    current_component.push(dest);

    while (!current_component.empty())
    {
        // Blocks part of next connected component by mining exactly 1
        // unexplored block.
        std::queue<position> next_component;

        // Explore the current connected component by moving without digging
        // any block.
        while (!current_component.empty())
        {
            position source = current_component.front();
            current_component.pop();

            static const direction directions[] = {HAUT, BAS, GAUCHE, DROITE};
            for (direction dir : directions)
            {
                const position target =
                    get_position_offset(source, (direction)dir);

                if (inside_map(target) &&
                    predecessor[target.ligne][target.colonne] ==
                        ERREUR_DIRECTION)
                {
                    switch (map_[target.ligne][target.colonne].type)
                    {
                    case LIBRE:
                        predecessor[target.ligne][target.colonne] = dir;
                        current_component.push(target);
                        break;
                    case GRANITE:
                        predecessor[target.ligne][target.colonne] = dir;
                        next_component.push(target);
                        break;
                    case OBSIDIENNE:
                    case ERREUR_CASE:
                        continue;
                    }

                    if (target == start)
                        goto reverse_run;
                }
            }
        }

        current_component = next_component;
    }

    // If the previous block is left without a goto, it means start has not been
    // reach
    return {};

reverse_run:

    // Unroll the path from the start.
    std::vector<direction> ret;
    position current_cell = start;

    while (current_cell != dest)
    {
        const direction dir = reverse_direction(
            predecessor[current_cell.ligne][current_cell.colonne]);
        current_cell = get_position_offset(current_cell, dir);
        ret.push_back(dir);
    }

    return ret;
}
