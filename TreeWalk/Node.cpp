#include "Node.h"

TreeWalk::Node::Node(void)
:
        node_(0)
{
}

TreeWalk::Node::Node(NodeCount n, size_t d)
:
        node_(0)
{
        node_ ^= (n & LEAFS_MASK) << LEAFS_SHIFT;
        node_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
}

bool TreeWalk::Node::is_depth_equal_to(size_t d) const
{
        return depth() == d; 
}

NodeCount TreeWalk::Node::leafs(void) const
{
        return (node_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

size_t TreeWalk::Node::depth(void) const
{
        return static_cast<size_t>((node_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}
