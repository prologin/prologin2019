#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "actions.hh"
#include "api.hh"
#include "constant.hh"
#include "game_state.hh"
#include "position.hh"
#include "rules.hh"

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
    ss << '"';
}

static std::ostream& operator<<(std::ostream& ss, const position& pos)
{
    ss << "{\"l\": " << pos.ligne << ", \"c\":" << pos.colonne << "}";
    return ss;
}

template <typename T>
static void dump_vector(std::vector<T> vector, std::ostream& ss,
                        std::function<void(std::ostream&, T)> func)
{
    bool is_first = true;
    ss << "[";
    for (T elem : vector)
    {
        if (!is_first)
            ss << ", ";
        func(ss, elem);
        is_first = false;
    }
    ss << "]";
}

static void dump_history(std::ostream& ss, const GameState& st, int player_id)
{
    dump_vector<internal_action>(
        st.get_internal_history(player_id), ss, [](auto& ss, auto action) {
            switch (action.type)
            {
            case 0:
                ss << "{\"action\": " << -1;
                ss << ", \"drapeau\": " << action.debug_flag.ftype;
                ss << ", \"pos\": " << action.debug_flag.pos << "}";
                break;
            case 1:
                ss << "{\"action\": " << action.action.atype;
                ss << ", \"id_nain\": " << action.action.id_nain;
                ss << ", \"dir\": " << action.action.dir;
                ss << ", \"sens\": " << action.action.sens << "}";
                break;
            case 2:
                ss << "{\"action\": " << -2;
                ss << ", \"player_id\": " << action.fall.player_id;
                ss << ", \"id_nain\": " << action.fall.nain_id;
                ss << ", \"goal\": " << action.fall.goal << "}";
                break;
            }
        });
}

static void dump_nains(std::ostream& ss, const GameState& st, int player_id)
{
    std::vector<std::pair<const nain*, int>> nains;
    for (int nain_id = 0; nain_id < NB_NAINS; ++nain_id)
    {
        const nain* nain = st.get_nain(player_id, nain_id);
        if (nain != nullptr)
            nains.push_back({nain, nain_id});
    }
    dump_vector<std::pair<const nain*, int>>(
        nains, ss, [](auto& ss, auto nain) {
            ss << "{\"id_nain\": " << nain.second << ", ";
            ss << "\"pos\": " << nain.first->pos << ", ";
            ss << "\"vie\": " << nain.first->vie << ", ";
            ss << "\"pa\": " << nain.first->pa << ", ";
            ss << "\"pm\": " << nain.first->pm << ", ";
            ss << "\"accroche\": " << nain.first->accroche << ", ";
            ss << "\"butin\": " << nain.first->butin << "}";
        });
}

static void dump_players(std::ostream& ss, const GameState& st)
{
    const auto& players = st.get_player_info();
    std::vector<std::pair<int, PlayerInfo>> players_vec;

    for (size_t id = 0; id < NB_JOUEURS; id++)
        players_vec.emplace_back(id, players[id]);

    dump_vector<std::pair<int, PlayerInfo>>(
        players_vec, ss, [&](auto& ss, auto player) {
            ss << "{ \"id\": " << player.first << ", \"name\": ";
            dump_string(ss, player.second.get_name());
            ss << ", \"score\": " << player.second.get_score();
            ss << ", \"nains\": ";
            dump_nains(ss, st, player.first);
            ss << ", \"historique\": ";
            dump_history(ss, st, player.first);
            ss << "}";
        });
}

static void dump_map(std::ostream& ss, const GameState& st)
{
    ss << "{\"cases\": ";
    std::vector<case_type> cells;
    for (int l = 0; l < TAILLE_MINE; l++)
        for (int c = 0; c < TAILLE_MINE; c++)
            cells.push_back(st.get_cell_type({l, c}));
    dump_vector<case_type>(cells, ss,
                           [](auto& ss, case_type cell) { ss << cell; });
    ss << ", \"minerais\": ";
    dump_vector<position>(st.get_ores(), ss, [&](auto& ss, position pos) {
        const minerai* ore = st.get_minerai(pos);
        ss << "{ \"pos\": " << pos;
        ss << ", \"resistance\": " << ore->resistance;
        ss << ", \"rendement\": " << ore->rendement << "}";
    });
    ss << ", \"cordes\": ";
    dump_vector<Rope>(st.get_base_ropes(), ss, [](auto& ss, Rope rope) {
        if (rope.get_positions().size() == 0)
            return;
        ss << "{ \"haut\": " << rope.get_anchor();
        ss << ", \"bas\": " << rope.get_bottom() << "}";
    });
    ss << "}";
}

static void dump_stream(std::ostream& ss, const GameState& st)
{
    ss << "{";
    ss << "\"tour\": [" << st.get_round() << ", " << NB_TOURS << "] ";
    ss << ", \"joueurs\": ";
    dump_players(ss, st);
    ss << ", \"carte\": ";
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
