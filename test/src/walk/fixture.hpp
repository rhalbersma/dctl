#pragma once
#include <dctl/utility/units.hpp>
#include <dctl/walk/root.hpp>           // Root
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <cstddef>                      // size_t

namespace dctl {
namespace walk {

auto const    unique_gen = successor::Successor<successor::select::legal, true >{};
auto const duplicate_gen = successor::Successor<successor::select::legal, false>{};

template<class Position, class Successor, int N>
void test(Position const& p, Successor successor, std::size_t const (& leafs)[N])
{
        static_assert(sizeof(Transposition) == 8, "");
        using impl_tag = hash_tag;
        Data<impl_tag, Position> d;
        Enhancements<impl_tag, Position> e(&d);
        e.resize_TT(64_MiB);

        auto depth = 0;
        for (auto&& node_count : leafs) {
                e.clear_TT();
                BOOST_CHECK_EQUAL(node_count, walk(p, ++depth, 0, successor, e));
        }
}

}       // namespace walk
}       // namespace dctl
