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
    void merge_down(const Rope& down);
    void clear();

    position get_anchor() const;
    position get_bottom() const;
    std::vector<position> get_positions() const;
};
