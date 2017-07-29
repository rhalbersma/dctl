#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/traversal/root.hpp> // perft_state
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL

namespace dctl::algo {
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
}       // namespace dctl::algo
