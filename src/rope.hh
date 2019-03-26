#pragma once

#include <vector>

#include "constant.hh"

class Rope
{
    std::vector<std::pair<int, int>> nains_;
    std::vector<position> pos_;

    public:
        Rope(position pos);

        void extends();

        std::vector<std::pair<int, int>> get_nains() const;
        position get_anchor() const;
        position get_bottom() const;
};
