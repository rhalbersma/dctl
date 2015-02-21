#pragma once
#include <dctl/walk/root.hpp>           // Root
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <cstddef>                      // size_t

namespace dctl {
namespace walk {

template<class Position, int N>
void test(Position const& p, std::size_t const (& leafs)[N])
{
        static_assert(sizeof(Transposition) == 8, "");
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
