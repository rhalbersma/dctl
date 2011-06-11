#include "Entry.h"

tree::walk::Entry::Entry(void)
:
        entry_(0)
{
}

tree::walk::Entry::Entry(NodeCount n, size_t d)
:
        entry_(0)
{
        entry_ ^= (n & LEAFS_MASK) << LEAFS_SHIFT;
        entry_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
}

NodeCount tree::walk::Entry::leafs(void) const
{
        return (entry_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

size_t tree::walk::Entry::depth(void) const
{
        return static_cast<size_t>((entry_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}

bool tree::walk::Entry::equal_to(size_t d) const
{
        return depth() == d; 
}
