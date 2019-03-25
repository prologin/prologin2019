#include "map.hh"

#include <algorithm>
#include <utils/log.hh>

#include "position.hh"

void Map::load_map_cells(std::istream& stream)
{
    for (int l = 0; l < TAILLE_MINE; ++l)
    {
        std::string line;
        stream >> line;
        if (line.length() != (size_t) TAILLE_MINE)
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
            case '|':
                map_[l][c] = LIBRE;
                rope_[l][c] = true;
                ropes_.push_back({ l, c });
                break;
            default:
                FATAL("Invalid cell type '%c' at (%d;%d)", line[c], l, c);
                break;
            }
        }
    }
}

void Map::load_minerai_info(std::istream& stream)
{
    std::vector<position> seen;
    int nb_minerai;
    stream >> nb_minerai;
    ores_.resize(nb_minerai);
    for (int minerai = 0; minerai < nb_minerai; ++minerai)
    {
        int l, c;
        stream >> l >> c;
        position pos = { l, c };

        std::string error;
        if (std::find(seen.cbegin(), seen.cend(), pos) == seen.cend())
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
            FATAL("resistance must be a strictly positif int");

        ores_[minerai] = { resistance, rendement };
        ore_[l][c] = &ores_[minerai];
        seen.push_back(pos);
    }
}

Map::Map(std::istream& stream)
{
    INFO("Loading map");

    load_map_cells(stream);
    load_minerai_info(stream);
}

Map::Map(const Map& map)
    : map_(map.map_)
    , rope_(map.rope_)
    , ropes_(map.ropes_)
    , ore_(map.ore_)
    , ores_(map.ores_)
{
}

case_type Map::get_cell_type(position pos) const
{
    if (!inside_map(pos))
        return ERREUR;
    return map_[pos.ligne][pos.colonne];
}

std::vector<position> Map::get_ropes() const
{
    return ropes_;
}

bool Map::is_rope(position pos) const
{
    if (!inside_map(pos))
        return false;
    return rope_[pos.ligne][pos.colonne];
}

minerai Map::get_minerrai(position pos) const
{
    if (!inside_map(pos))
        return { -1, -1 };
    return *ore_[pos.ligne][pos.colonne];
}

void Map::set_cell_type(position pos, case_type type)
{
    map_[pos.ligne][pos.colonne] = type;
}
