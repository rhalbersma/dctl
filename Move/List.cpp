#include "List.h"
#include "../Utilities/Bit.h"
#include <algorithm>

size_t Move::List::size(void) const
{
        return move_list_.size();
}

const Pieces& Move::List::operator[](size_t i) const
{
        return move_list_[i];
}

bool Move::List::empty(void) const
{
        return move_list_.empty();
}

// specialization for move generation without duplicate capture checking
bool Move::List::non_unique_back(Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool Move::List::non_unique_back(Int2Type<true>)
{
        return std::find(move_list_.begin(), move_list_.end(), move_list_.back()) != (move_list_.end() - 1);
}

void Move::List::pop_back(void)
{
        return move_list_.pop_back();
}

void Move::List::clear(void)
{
        return move_list_.clear();
}
