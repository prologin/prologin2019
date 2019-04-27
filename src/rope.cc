#include "rope.hh"

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
