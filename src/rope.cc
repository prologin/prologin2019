#include "rope.hh"

#include <algorithm>

#include "position.hh"

Rope::Rope(position pos)
{
    pos_.push_back(pos);
}

const std::vector<std::pair<int, int>>& Rope::get_nains() const
{
    return nains_;
}

void Rope::extends(position pos)
{
    pos_.push_back(pos);
}

void Rope::merge_up(const Rope& rope)
{
    const auto& poss(rope.get_positions());

    for (auto pos = poss.rbegin(); pos != poss.rend(); ++pos)
        pos_.insert(pos_.begin(), *pos);
}

void Rope::remove_nain(int player_id, int nain_id)
{
    nains_.erase(std::remove(nains_.begin(), nains_.end(),
                             std::make_pair(player_id, nain_id)),
                 nains_.end());
}
void Rope::add_nain(int player_id, int nain_id)
{
    nains_.push_back({player_id, nain_id});
}

void Rope::clear()
{
    pos_.clear();
}

position Rope::get_anchor() const
{
    return pos_.front();
}

position Rope::get_bottom() const
{
    return pos_.back();
}

std::vector<position> Rope::get_positions() const
{
    return pos_;
}
