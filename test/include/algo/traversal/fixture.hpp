#pragma once
#include <dctl/algo/traversal/root.hpp> // Root
#include <dctl/core/actions.hpp>
#include <dctl/util/units.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL

namespace dctl {
namespace algo {
namespace traversal {

template<class Actions, class State, class Range>
void test(Actions const& successor, State const& s, Range const& leafs)
{
        auto depth = 0;
        for (auto const& node_count : leafs) {
                BOOST_CHECK_EQUAL(node_count, perft_state<true>(successor, s, ++depth));
        }
}

}       // namespace traversal
}       // namespace algo
}       // namespace dctl
