#pragma once

#include <vector>

#include "constant.hh"

class Rope
{
    std::vector<std::pair<int, int>> nains_;
    std::vector<position> pos_;

    public:
        Rope(position pos);

        void extends(position pos);
        void merge_up(const Rope *rope);
        void remove_nain(int player_id, int nain_id);
        void add_nain(int player_id, int nain_id);

        std::vector<std::pair<int, int>> get_nains() const;

        position get_anchor() const;
        position get_bottom() const;
        std::vector<position> get_positions() const;
};
