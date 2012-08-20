#pragma once
#include <algorithm>                    // std:max
#include "../utility/IntegerTypes.hpp"

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
        {
                cycle_begin.reserve(100);
                cycle_begin.assign(100, 0);

                cycle_end.reserve(100);
                cycle_end.assign(100, 0);

                cycle_size.reserve(100);
                cycle_size.assign(100, 0);
        }

        // modifiers

        void reset()
        {
                nodes_ = sum_ply_ = max_ply_ = 0;
                cycle_begin.assign(100, 0);
                cycle_end.assign(100, 0);
                cycle_size.assign(100, 0);
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
                return nodes_;
        }

        NodeCount sum_ply() const
        {
                return sum_ply_;
        }

        int max_ply() const
        {
                return max_ply_;
        }

        std::vector<NodeCount> cycle_begin;
        std::vector<NodeCount> cycle_end;
        std::vector<NodeCount> cycle_size;

private:
        // representation

        NodeCount nodes_;
        NodeCount sum_ply_;
        int max_ply_;
};

}       // namespace dctl
