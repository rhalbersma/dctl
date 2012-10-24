#pragma once
#include <algorithm>                    // std:max
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {

class Statistics
{
public:
        // structors

        Statistics()
        :
                nodes_(0),
                sum_ply_(0),
                max_ply_(0)
        {}

        // modifiers

        void reset()
        {
                nodes_ = sum_ply_ = max_ply_ = 0;
        }

        void update(int ply)
        {
                ++nodes_;
                sum_ply_ += ply;
                max_ply_ = std::max(ply, max_ply_);
        }

        // queries

        NodeCount nodes() const
        {
                return (nodes_);
        }

        NodeCount sum_ply() const
        {
                return (sum_ply_);
        }

        int max_ply() const
        {
                return (max_ply_);
        }

private:
        // representation

        NodeCount nodes_;
        NodeCount sum_ply_;
        int max_ply_;
};

}       // namespace dctl
