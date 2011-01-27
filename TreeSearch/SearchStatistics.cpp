#include "SearchStatistics.h"
#include <algorithm>

NodeCount SearchStatistics::nodes(void) const
{
        return nodes_;
}

NodeCount SearchStatistics::sum_ply(void) const
{
        return sum_ply_;
}

size_t SearchStatistics::max_ply(void) const
{
        return max_ply_;
}

void SearchStatistics::reset(void)
{
        nodes_ = sum_ply_ = max_ply_ = 0;
}

void SearchStatistics::update(size_t ply)
{
        ++nodes_;
        sum_ply_ += ply;
        max_ply_ = std::max(ply, max_ply_);
}
