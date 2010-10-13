#include "SearchStatistics.h"
#include <algorithm>

uint64_t SearchStatistics::nodes(void) const
{
        return d_nodes;
}

uint64_t SearchStatistics::sum_ply(void) const
{
        return d_sum_ply;
}

size_t SearchStatistics::max_ply(void) const
{
        return d_max_ply;
}

void SearchStatistics::reset(void)
{
        d_nodes = d_sum_ply = d_max_ply = 0;
}

void SearchStatistics::update(void)
{
        ++d_nodes;
}

void SearchStatistics::update(size_t ply)
{
        update();
        d_sum_ply += ply;
        d_max_ply = std::max(ply, d_max_ply);
}
