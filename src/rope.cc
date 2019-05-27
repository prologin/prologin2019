#include "rope.hh"

#include <algorithm>

#include "position.hh"

Rope::Rope(position pos)
{
    pos_.push_back(pos);
}

void Rope::extends(position pos)
{
    pos_.push_back(pos);
}

void Rope::merge_down(const Rope& other)
{
    pos_.reserve(pos_.size() + other.pos_.size());
    pos_.insert(pos_.end(), other.pos_.begin(), other.pos_.end());
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
