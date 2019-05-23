#include "map.hh"

#include <algorithm>
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
                map_[l][c] = LIBRE;
                break;
            case 'X':
                map_[l][c] = GRANITE;
                break;
            case '#':
                map_[l][c] = OBSIDIENNE;
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
            map_[down.ligne][down.colonne] = OBSIDIENNE;
    }
}

void Map::load_minerai_info(std::istream& stream)
{
    std::vector<position> seen;
    int nb_minerai;
    stream >> nb_minerai;
    ores_.resize(nb_minerai);
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
        if (map_[l][c] != GRANITE)
            error = "not in granite";

        if (error != "")
            FATAL("invalid position (%d;%d) for minerai is %s", l, c,
                  error.c_str());

        int resistance, rendement;
        stream >> resistance >> rendement;

        if (resistance <= 0)
            FATAL("resistance must be a strictly positive int");

        ores_[minerai] = {resistance, rendement};
        ores_pos_[minerai] = {l, c};
        ore_[l][c] = minerai;
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
        if (map_[l][c] != LIBRE)
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
        {
            ore_[y][x] = -1;
            rope_[y][x] = -1;
            nains_[y][x] = {-1, std::unordered_set<int>()};
        }

    load_map_cells(stream);
    load_spawn_point(stream);
    load_minerai_info(stream);
    load_rope_info(stream);
}

Map::Map(const Map& map)
    : nains_(map.nains_)
    , map_(map.map_)
    , spawn_point_(map.spawn_point_)
    , rope_(map.rope_)
    , ropes_(map.ropes_)
    , ore_(map.ore_)
    , ores_(map.ores_)
    , ores_pos_(map.ores_pos_)
{
}

position Map::get_spawn_point(int player_id) const
{
    return spawn_point_[player_id];
}

case_type Map::get_cell_type(position pos) const
{
    if (!inside_map(pos))
        return ERREUR_CASE;
    return map_[pos.ligne][pos.colonne];
}

const minerai* Map::get_minerai(position pos) const
{
    if (!inside_map(pos))
        return nullptr;
    int index = ore_[pos.ligne][pos.colonne];
    if (index == -1)
        return nullptr;
    return &ores_[index];
}

const std::vector<position>& Map::get_ores() const
{
    return ores_pos_;
}

void Map::set_cell_type(position pos, case_type type)
{
    map_[pos.ligne][pos.colonne] = type;
}

inline bool operator==(const minerai& a, const minerai& b)
{
    return a.resistance == b.resistance && a.rendement == b.rendement;
}

void Map::remove_minerai(position pos)
{
    ores_pos_.erase(std::remove(ores_pos_.begin(), ores_pos_.end(), pos),
                    ores_pos_.end());
    ore_[pos.ligne][pos.colonne] = -1;
}

void Map::set_minerai_resistance(position pos, int resistance)
{
    ores_[ore_[pos.ligne][pos.colonne]].resistance = resistance;
}

void Map::add_nain(int nain_id, position pos, int player_id)
{
    if (nains_[pos.ligne][pos.colonne].ids.empty())
        nains_[pos.ligne][pos.colonne].player = player_id;

    nains_[pos.ligne][pos.colonne].ids.insert(nain_id);
}

void Map::move_nain(int nain_id, position from, position to)
{
    const int player_id = nains_[from.ligne][from.colonne].player;
    add_nain(nain_id, to, player_id);
    remove_nain(nain_id, from);
}

void Map::remove_nain(int nain_id, position pos)
{
    nains_[pos.ligne][pos.colonne].ids.erase(nain_id);

    if (nains_[pos.ligne][pos.colonne].ids.empty())
        nains_[pos.ligne][pos.colonne].player = -1;
}

const NainsOnCell& Map::get_nains_at(position pos) const
{
    return nains_[pos.ligne][pos.colonne];
}

inline bool operator==(const Rope& a, const Rope& b)
{
    return a.get_anchor() == b.get_anchor() && a.get_bottom() == b.get_bottom();
}

void Map::add_rope(position pos)
{
    ropes_.push_back(Rope(pos));
    rope_[pos.ligne][pos.colonne] = ropes_.size() - 1;
}

const Rope* Map::get_rope(position pos) const
{
    if (!inside_map(pos))
        return nullptr;
    int index = rope_[pos.ligne][pos.colonne];
    if (index == -1)
        return nullptr;
    return &ropes_[index];
}

void Map::add_nain_to_rope(position pos, int player_id, int nain_id)
{
    ropes_[rope_[pos.ligne][pos.colonne]].add_nain(player_id, nain_id);
}

void Map::remove_nain_from_rope(position pos, int player_id, int nain_id)
{
    ropes_[rope_[pos.ligne][pos.colonne]].remove_nain(player_id, nain_id);
}

std::vector<position> Map::get_ropes_positions() const
{
    std::vector<position> result;

    for (const Rope rope : get_base_ropes())
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
    const Rope* rope = get_rope(pos);

    if (rope == nullptr)
        return false;

    position dest = get_position_offset(rope->get_bottom(), BAS);
    if (!inside_map(dest) || get_cell_type(dest) != LIBRE)
        return false;

    if (get_rope(dest) == nullptr)
    {
        ropes_[rope_[pos.ligne][pos.colonne]].extends(dest);
        rope_[dest.ligne][dest.colonne] = rope_[pos.ligne][pos.colonne];
        return true;
    }

    // Merge with the bottom rope
    ropes_[rope_[dest.ligne][dest.colonne]].merge_up(rope);
    const auto& positions(rope->get_positions());
    ropes_[rope_[pos.ligne][pos.colonne]].clear();

    for (position pos : positions)
        rope_[pos.ligne][pos.colonne] = rope_[dest.ligne][dest.colonne];

    for (const auto& nain : rope->get_nains())
        add_nain_to_rope(dest, nain.first, nain.second);

    return false;
}

std::vector<direction> Map::get_shortest_path(position start,
                                              position dest) const
{
    if (!inside_map(start) || !inside_map(dest) || start == dest)
        return {};

    auto pos_id = [](position pos) -> int {
        return pos.ligne * TAILLE_MINE + pos.colonne;
    };

    auto pos_from_id = [](int id) -> position {
        position pos;
        pos.ligne = id / TAILLE_MINE;
        pos.colonne = id % TAILLE_MINE;
        return pos;
    };

    const int dest_id = pos_id(dest);
    const int start_id = pos_id(start);

    // Keep track of the predecessor of each cell in a shortest path from start
    // to any cell.
    std::array<direction, TAILLE_MINE * TAILLE_MINE> predecessor;
    predecessor.fill(ERREUR_DIRECTION);

    // Current component to explore: the queue is ordered in increasing number
    // of moves to use, all cells in this queue are accessible with the same
    // minimal number of mined blocks (initialy 0).
    std::queue<int> current_component;
    current_component.push(pos_id(start));

    while (!current_component.empty())
    {
        // Blocks part of next connected component by mining exactly 1
        // unexplored block.
        std::queue<int> next_component;

        // Explore the current connected component by moving without digging
        // any block.
        while (!current_component.empty())
        {
            int source_id = current_component.front();
            current_component.pop();
            position source = pos_from_id(source_id);

            static const direction directions[] = {GAUCHE, DROITE, HAUT, BAS};
            for (direction dir : directions)
            {
                const auto target = get_position_offset(source, (direction)dir);
                const int target_id = pos_id(target);

                if (inside_map(target) &&
                    predecessor[target_id] == ERREUR_DIRECTION)
                {
                    switch (map_[target.ligne][target.colonne])
                    {
                    case LIBRE:
                        predecessor[target_id] = dir;
                        current_component.push(target_id);
                        break;
                    case GRANITE:
                        predecessor[target_id] = dir;
                        next_component.push(target_id);
                        break;
                    case OBSIDIENNE:
                    case ERREUR_CASE:
                        continue;
                    }

                    if (target_id == dest_id)
                        goto reverse_run;
                }
            }
        }

        current_component = next_component;
    }

    // There might be no path to a cell if it is surrounded by unbreakable
    // blocks.
    if (predecessor[dest_id] == ERREUR_DIRECTION)
        return {};

reverse_run:

    // Unroll the path from the end.
    std::vector<direction> ret;
    int current_cell_id = dest_id;

    while (current_cell_id != start_id)
    {
        const direction dir = predecessor[current_cell_id];
        current_cell_id = pos_id(get_position_offset(
            pos_from_id(current_cell_id), reverse_direction(dir)));
        ret.push_back(dir);
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}
