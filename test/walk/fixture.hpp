#pragma once
#include <cstddef>                      // size_t
#include <boost/test/unit_test.hpp>	// BOOST_CHECK_EQUAL
#include <dctl/walk/root.hpp>           // Root
#include <dctl/utility/int.hpp>        	// NodeCount

namespace dctl {
namespace walk {

template<typename Position, std::size_t N>
void test(Position const& p, NodeCount const (&/*leafs*/)[N])
{
        Enhancements<hash_tag, Position> e;
/*
        auto depth = 0;
        for (auto const& node_count: leafs) {
                e.handle_->TT_.clear();
                BOOST_CHECK_EQUAL(node_count, walk(e, p, ++depth));
        }
*/
        perft(e, p, 16);
}

}       // namespace walk
}       // namespace dctl
