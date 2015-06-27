#pragma once
#include <dctl/utility/units.hpp>
#include <dctl/walk/root.hpp>           // Root
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <cstddef>                      // size_t

namespace dctl {
namespace walk {

auto const    unique_gen = actions::Successor<actions::select::legal, true >{};
auto const duplicate_gen = actions::Successor<actions::select::legal, false>{};

template<class State, class Successor, class Range>
void test(State const& p, Successor successor, Range const& leafs)
{
        static_assert(sizeof(Transposition) == 8, "");
        using impl_tag = hash_tag;
        Data<impl_tag, State> d;
        Enhancements<impl_tag, State> e(&d);
        e.resize_TT(64_MiB);

        auto depth = 0;
        for (auto&& node_count : leafs)
                BOOST_CHECK_EQUAL(node_count, walk(p, ++depth, 0, successor, e));
}

}       // namespace walk
}       // namespace dctl
