#include <algorithm>
#include "Statistics.h"

Tree::Statistics::Statistics(void)
:
        nodes_(0),
        sum_ply_(0),
        max_ply_(0)
{
}

NodeCount Tree::Statistics::nodes(void) const
{
        return nodes_;
}

NodeCount Tree::Statistics::sum_ply(void) const
{
        return sum_ply_;
}

size_t Tree::Statistics::max_ply(void) const
{
        return max_ply_;
}

void Tree::Statistics::reset(void)
{
        nodes_ = sum_ply_ = max_ply_ = 0;
}

void Tree::Statistics::update(size_t ply)
{
        ++nodes_;
        sum_ply_ += ply;
        max_ply_ = std::max(ply, max_ply_);
}
