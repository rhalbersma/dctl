#include "Stack.h"
#include "../../Utilities/Bit.h"
#include <algorithm>

size_t Tree::Move::Stack::size(void) const
{
        return vector_array_.size();
}

bool Tree::Move::Stack::empty(void) const
{
        return vector_array_.empty();
}

Tree::Node::Pieces& Tree::Move::Stack::operator[](size_t i)
{
        return vector_array_[i];
}

const Tree::Node::Pieces& Tree::Move::Stack::operator[](size_t i) const
{
        return vector_array_[i];
}

Tree::Node::Pieces& Tree::Move::Stack::top(void)
{
        return vector_array_.back();
}

const Tree::Node::Pieces& Tree::Move::Stack::top(void) const
{
        return vector_array_.back();
}

// specialization for move generation without duplicate capture checking
bool Tree::Move::Stack::non_unique_top(Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool Tree::Move::Stack::non_unique_top(Int2Type<true>)
{
        return std::find(vector_array_.begin(), vector_array_.end(), top()) != (vector_array_.end() - 1);
}

void Tree::Move::Stack::pop(void)
{
        return vector_array_.pop_back();
}

void Tree::Move::Stack::clear(void)
{
        return vector_array_.clear();
}
