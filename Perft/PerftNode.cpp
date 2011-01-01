#include "PerftNode.h"

PerftNode::PerftNode(void)
:
        perft_node_(0)
{
}

PerftNode::PerftNode(NodeCount s_leafs, size_t s_depth)
:
        perft_node_(0)
{
        perft_node_ ^= (s_leafs & LEAFS_MASK) << LEAFS_SHIFT;
        perft_node_ ^= (s_depth & DEPTH_MASK) << DEPTH_SHIFT;
}

bool PerftNode::is_depth_equal_to(size_t s_depth) const
{
        return depth() == s_depth; 
}

NodeCount PerftNode::leafs(void) const
{
        return (perft_node_ & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

size_t PerftNode::depth(void) const
{
        return static_cast<size_t>((perft_node_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}
