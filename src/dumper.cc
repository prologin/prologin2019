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

#include <iomanip>
#include <iostream>
#include <string>

#include "actions.hh"
#include "api.hh"
#include "constant.hh"
#include "game_state.hh"
#include "position.hh"
#include "rules.hh"

constexpr auto COMMA = ", ";

/// Decodes a UTF-8 string to a list of 32 bit unicode codepoints. Ignores
/// erroneous characters.
/// Imported from prologin2016
static std::u32string utf8_decode(const std::string& s)
{
    std::u32string ret;
    size_t i = 0;
    size_t size = s.size();

    while (i < size)
    {
        if ((s[i] & 0x80) == 0)
        {
            ret.push_back(s[i++]);
        }
        else if ((s[i] & 0xe0) == 0xc0 && (i + 1) < size &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x1f) << 6) | (s[i + 1] & 0x3f));
            i += 2;
        }
        else if ((s[i] & 0xf0) == 0xe0 && (i + 2) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x0f) << 12) | ((s[i + 1] & 0x3f) << 6) |
                          (s[i + 2] & 0x3f));
            i += 3;
        }
        else if ((s[i] & 0xf8) == 0xf0 && (i + 3) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80 &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x07) << 18) | ((s[i + 1] & 0x3f) << 12) |
                          ((s[i + 2] & 0x3f) << 6) | (s[i + 3] & 0x3f));
            i += 4;
        }
        else
        {
            i++;
        }
    }

    return ret;
}

/// Dump a JSON-escaped UTF-8 string
/// Imported from prologin2016
static void dump_string(std::ostream& ss, const std::string& s)
{
    /*
     * RFC4627, 2.5:
     * All Unicode characters may be placed within the quotation marks except
     * for the characters that must be escaped: quotation mark, reverse solidus,
     * and the control characters (U+0000 through U+001F).
     */
    std::ios state(nullptr);
    state.copyfmt(ss);
    std::u32string utf32 = utf8_decode(s);
    ss << "\"";
    for (char32_t& c : utf32)
    {
        if (c == u'"')
        {
            ss << "\\\"";
        }
        else if (c == u'\\')
        {
            ss << "\\\\";
        }
        else if (u'\u0020' <= c && c <= u'\u007E')
        {
            // printable ASCII
            ss << static_cast<char>(c);
        }
        else if (c > u'\uFFFF')
        {
            // surrogate pair
            // http://unicode.org/faq/utf_bom.html#utf16-2
            const unsigned s = c - 0x010000;
            const unsigned lead = (s >> 10) + 0xD800;
            const unsigned trail = (s & 0x3FF) + 0xDC00;
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << lead;
            ss.copyfmt(state);
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << trail;
            ss.copyfmt(state);
        }
        else
        {
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << c;
            ss.copyfmt(state);
        }
    }
    ss << "\"";
}

static std::ostream& operator<<(std::ostream& ss, const position& pos)
{
    ss << "{\"r\": " << pos.ligne << ", \"c\":" << pos.colonne << "}";
    return ss;
}

static std::ostream& operator<<(std::ostream& ss, action_id action_type)
{
    ss << "\"";
    switch (action_type)
    {
    case ID_ACTION_DEPLACER:
        ss << "ID_ACTION_DEPLACER";
        break;
    case ID_ACTION_ESCALADER:
        ss << "ID_ACTION_ESCALADER";
        break;
    case ID_ACTION_GRIMPER:
        ss << "ID_ACTION_GRIMPER";
        break;
    case ID_ACTION_LACHER:
        ss << "ID_ACTION_LACHER";
        break;
    case ID_ACTION_MINER:
        ss << "ID_ACTION_MINER";
        break;
    case ID_ACTION_TIRER:
        ss << "ID_ACTION_TIRER";
        break;
    case ID_ACTION_POSER_CORDE:
        ss << "ID_ACTION_POSER_CORDE";
        break;
    case ID_ACTION_DEBUG_AFFICHER_DRAPEAU:
        ss << "ID_ACTION_DEBUG_AFFICHER_DRAPEAU";
        break;
    }
    ss << "\"";
    return ss;
}

static void dump_nains(std::ostream& ss, const GameState& st, int player_id)
{
    auto sep = "";
    ss << "[";
    for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
    {
        ss << sep;
        sep = COMMA;

        position nain_pos = st.get_nain(player_id, nain_id).pos;
        ss << "{\"id_nain\": " << nain_id << ", "
           << "\"pos\": " << nain_pos << "}";
    }
    ss << "]";
}

static void dump_players(std::ostream& ss, const GameState& st)
{
    const auto& players = st.get_player_info();

    auto sep = "";
    ss << "{";
    for (const auto& player_info : players)
    {
        ss << sep;
        sep = COMMA;

        int player_id = player_info.first;
        const auto& player = player_info.second;
        ss << "\"" << player_id << "\": {"
           << "\"name\": ";
        dump_string(ss, player.get_name());
        ss << ", \"score\": " << player.get_score();

        ss << ", \"nains\": ";
        dump_nains(ss, st, player_id);

        ss << "}";
    }
    ss << "}";
}

static void dump_map(std::ostream& ss, const GameState& st)
{
    ss << "{"
       << "\"cells\": [";
    for (int l = 0; l < TAILLE_MINE; l++)
    {
        for (int c = 0; c < TAILLE_MINE; c++)
        {
            position pos{l, c};
            ss << st.get_cell_type(pos);
            if (!(l == TAILLE_MINE - 1 && c == TAILLE_MINE - 1))
                ss << ", ";
        }
    }

    ss << "]";
    ss << "}";
}

static void dump_stream(std::ostream& ss, const GameState& st)
{
    ss << "{";
    ss << "\"round\": [" << st.get_round() << ", " << NB_TOURS << "] ";

    ss << ", \"players\": ";
    dump_players(ss, st);

    ss << ", \"map\": ";
    dump_map(ss, st);

    ss << "}\n";
}

void Rules::dump_state(std::ostream& ss)
{
    dump_stream(ss, *api_->game_state());
}

// from api.cc
extern Api* api;

extern "C" const char* dump_state_json()
{
    // Warning: everytime this function is called, it invalidates the previous
    // return values by free-ing them.
    static std::string s;
    std::ostringstream ss;
    dump_stream(ss, *api->game_state());
    s = ss.str();
    return s.c_str();
}
