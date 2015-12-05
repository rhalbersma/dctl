#pragma once
#include <dctl/utility/units.hpp>
#include <dctl/ai/traversal/root.hpp>           // Root
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <cstddef>                      // size_t

namespace dctl {
namespace traversal {

auto const    unique_gen = actions::Successor<actions::select::legal, true >{};
auto const duplicate_gen = actions::Successor<actions::select::legal, false>{};

template<class State, class Successor, class Range>
void test(State const& s, Successor successor, Range const& leafs)
{
        auto walker = XWalk<State, Successor>{successor};

        auto depth = 0;
        for (auto&& node_count : leafs) {
                BOOST_CHECK_EQUAL(node_count, walker.run(s, ++depth));
        }
}

}       // namespace traversal
}       // namespace dctl
