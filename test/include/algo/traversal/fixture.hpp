#pragma once
#include <dctl/algo/traversal/root.hpp> // Root
#include <dctl/util/units.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL

namespace dctl {
namespace aima {
namespace traversal {

auto const keep_duplicates_gen = Actions<select::legal, keep_duplicates_tag>{};
auto const drop_duplicates_gen = Actions<select::legal, drop_duplicates_tag>{};

template<class Actions, class State, class Range>
void test(Actions const& successor, State const& s, Range const& leafs)
{
        auto depth = 0;
        for (auto const& node_count : leafs) {
                BOOST_CHECK_EQUAL(node_count, perft_state<true>(successor, s, ++depth));
        }
}

}       // namespace traversal
}       // namespace aima
}       // namespace dctl