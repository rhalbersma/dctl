#include <algorithm>    // std:max
#include "Statistics.h"

namespace tree {

Statistics::Statistics()
:
        nodes_(0),
        sum_ply_(0),
        max_ply_(0)
{
}

NodeCount Statistics::nodes() const
{
        return nodes_;
}

NodeCount Statistics::sum_ply() const
{
        return sum_ply_;
}

size_t Statistics::max_ply() const
{
        return max_ply_;
}

void Statistics::reset()
{
        nodes_ = sum_ply_ = max_ply_ = 0;
}

void Statistics::update(size_t ply)
{
        ++nodes_;
        sum_ply_ += ply;
        max_ply_ = std::max(ply, max_ply_);
}

}       // namespace tree
