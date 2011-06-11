#include <algorithm>
#include "Stack.h"
#include "../../Utilities/Bit.h"

size_t tree::move::Stack::size(void) const
{
        return vector_array_.size();
}

bool tree::move::Stack::empty(void) const
{
        return vector_array_.empty();
}

tree::node::Pieces& tree::move::Stack::operator[](size_t i)
{
        return vector_array_[i];
}

const tree::node::Pieces& tree::move::Stack::operator[](size_t i) const
{
        return vector_array_[i];
}

tree::node::Pieces& tree::move::Stack::top(void)
{
        return vector_array_.back();
}

const tree::node::Pieces& tree::move::Stack::top(void) const
{
        return vector_array_.back();
}

// specialization for move generation without duplicate capture checking
bool tree::move::Stack::non_unique_top(Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool tree::move::Stack::non_unique_top(Int2Type<true>)
{
        return std::find(vector_array_.begin(), vector_array_.end(), top()) != (vector_array_.end() - 1);
}

void tree::move::Stack::pop(void)
{
        return vector_array_.pop_back();
}

void tree::move::Stack::clear(void)
{
        return vector_array_.clear();
}
