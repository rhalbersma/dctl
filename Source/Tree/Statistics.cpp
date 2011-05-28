#include "Statistics.h"
#include <algorithm>

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

NodeCount Tree::Statistics::pv(void) const
{
        return pv_;
}

void Tree::Statistics::reset(void)
{
        nodes_ = sum_ply_ = pv_ = max_ply_ = 0;
}

void Tree::Statistics::update(size_t ply)
{
        ++nodes_;
        sum_ply_ += ply;
        max_ply_ = std::max(ply, max_ply_);
}

void Tree::Statistics::update_pv(void)
{
        ++pv_;
}
