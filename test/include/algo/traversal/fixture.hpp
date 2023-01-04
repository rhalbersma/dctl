#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/traversal/root.hpp> // depth_limited_count
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL

namespace dctl::algo::traversal {

void test(auto const& state, auto const& node_counts, auto const& model)
{
        for (auto depth = 1; auto const& node_count : node_counts) {
                BOOST_CHECK_EQUAL(node_count, depth_limited_count<true>(state, depth++, model));
        }
}

}       // namespace dctl::algo::traversal
