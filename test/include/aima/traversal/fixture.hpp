#pragma once
#include <dctl/utility/units.hpp>
#include <dctl/aima/traversal/root.hpp>   // Root
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <cstddef>                      // size_t

namespace dctl {
namespace aima {
namespace traversal {

auto const keep_duplicates_gen = core::Actions<core::select::legal, keep_duplicates_tag>{};
auto const drop_duplicates_gen = core::Actions<core::select::legal, drop_duplicates_tag>{};

template<class State, class Actions, class Range>
void test(State const& s, Actions successor, Range const& leafs)
{
        auto depth = 0;
        for (auto&& node_count : leafs) {
                BOOST_CHECK_EQUAL(node_count, perft_bulk_counting(successor, s, ++depth));
        }
}

}       // namespace traversal
}       // namespace aima
}       // namespace dctl
