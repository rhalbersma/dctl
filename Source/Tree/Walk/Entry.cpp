#include "Entry.h"

Tree::Walk::Entry::Entry(void)
:
        entry_(0)
{
}

Tree::Walk::Entry::Entry(NodeCount n, size_t d)
:
        entry_(0)
{
        entry_ ^= (n & LEAFS_MASK) << LEAFS_SHIFT;
        entry_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
}

NodeCount Tree::Walk::Entry::leafs(void) const
{
        return (entry_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

size_t Tree::Walk::Entry::depth(void) const
{
        return static_cast<size_t>((entry_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}

bool Tree::Walk::Entry::equal_to(size_t d) const
{
        return depth() == d; 
}
