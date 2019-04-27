#include "rope.hh"

#include <algorithm>

#include "position.hh"

Rope::Rope(position pos)
    : pos_(1, pos)
{ }

std::vector<std::pair<int, int>> Rope::get_nains() const
{
    return nains_;
}

void Rope::extends()
{
    pos_.push_back(get_position_offset(pos_.back(), BAS));
}

void Rope::merge_up(const Rope *rope)
{
    for (auto pos = rope->get_positions().cbegin(); pos != rope->get_positions().cend(); ++pos)
        pos_.insert(pos_.begin(), *pos);
}

void Rope::remove_nain(int player_id, int nain_id)
{
    std::remove(nains_.begin(), nains_.end(), std::make_pair(player_id, nain_id));
}
void Rope::add_nain(int player_id, int nain_id)
{
    nains_.push_back({ player_id, nain_id });
}

position Rope::get_anchor() const
{
    return pos_.front();
}

position Rope::get_bottom() const
{
    return pos_.back();
}

const std::vector<position>& Rope::get_positions() const
{
    return pos_;
}
