#include <algorithm>
#include "Statistics.h"

tree::Statistics::Statistics(void)
:
        nodes_(0),
        sum_ply_(0),
        max_ply_(0)
{
}

NodeCount tree::Statistics::nodes(void) const
{
        return nodes_;
}

NodeCount tree::Statistics::sum_ply(void) const
{
        return sum_ply_;
}

size_t tree::Statistics::max_ply(void) const
{
        return max_ply_;
}

void tree::Statistics::reset(void)
{
        nodes_ = sum_ply_ = max_ply_ = 0;
}

void tree::Statistics::update(size_t ply)
{
        ++nodes_;
        sum_ply_ += ply;
        max_ply_ = std::max(ply, max_ply_);
}
