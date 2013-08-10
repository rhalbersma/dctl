#pragma once
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

class Statistics
{
public:
        using NodeStats = boost::accumulators::accumulator_set<NodeCount, boost::accumulators::stats<boost::accumulators::tag::count> >;
        using PlyStats = boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::max> >;

        // modifiers

        void reset()
        {
                nodes_ = NodeStats();
                ply_ = PlyStats();
        }

        void collect(int ply)
        {
                nodes_(1);
                ply_(ply);
        }

        // queries

        NodeStats nodes() const
        {
                return nodes_;
        }

        PlyStats ply() const
        {
                return ply_;
        }

private:
        // representation

        NodeStats nodes_;
        PlyStats ply_;
};

}       // namespace dctl
