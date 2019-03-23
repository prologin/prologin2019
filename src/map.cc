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
    minerai_.resize(nb_minerai);
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

        minerai_[minerai] = { resistance, rendement };
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
    , minerai_(map.minerai_)
{
}

case_type Map::get_cell_type(position pos) const
{
    if (inside_map(pos))
        return map_[pos.ligne][pos.colonne];
    else
        return ERREUR;
}
