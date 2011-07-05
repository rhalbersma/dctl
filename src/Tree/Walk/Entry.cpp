#include "Entry.h"

tree::walk::Entry::Entry()
:
        entry_(0)
{
}

tree::walk::Entry::Entry(NodeCount n, int d)
:
        entry_(0)
{
        entry_ ^= (n & LEAFS_MASK) << LEAFS_SHIFT;
        entry_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
}

NodeCount tree::walk::Entry::leafs() const
{
        return (entry_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

int tree::walk::Entry::depth() const
{
        return static_cast<int>((entry_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}
