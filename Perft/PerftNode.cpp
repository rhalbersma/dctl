#include "PerftNode.h"

PerftNode::PerftNode(void)
:
        d_perft_node(0)
{
}

PerftNode::PerftNode(uint64_t s_leafs, size_t s_depth)
:
        d_perft_node(0)
{
        d_perft_node ^= (s_leafs & LEAFS_MASK) << LEAFS_SHIFT;
        d_perft_node ^= (s_depth & DEPTH_MASK) << DEPTH_SHIFT;
}

bool PerftNode::is_depth_equal_to(size_t s_depth) const
{
        return depth() == s_depth; 
}

uint64_t PerftNode::leafs(void) const
{
        return (d_perft_node & (LEAFS_MASK << LEAFS_SHIFT)) >> LEAFS_SHIFT;
}

size_t PerftNode::depth(void) const
{
        return static_cast<size_t>((d_perft_node & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}
