#pragma once
#include <dctl/walk/root.hpp>           // Root
#include <dctl/utility/int.hpp>        	// NodeCount
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL

namespace dctl {
namespace walk {

template<class Position, int N>
void test(Position const& p, NodeCount const (& leafs)[N])
{
        using impl_tag = hash_tag;
        Data<impl_tag, Position> d;
        Enhancements<impl_tag, Position> e(&d);
        e.resize_TT(64);

        auto depth = 0;
        for (auto&& node_count : leafs) {
                e.clear_TT();
                BOOST_CHECK_EQUAL(node_count, walk(p, ++depth, 0, e));
        }
}

}       // namespace walk
}       // namespace dctl
