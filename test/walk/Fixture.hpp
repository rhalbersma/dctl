#pragma once
#include <boost/test/unit_test.hpp>
#include <iterator>
#include "../../src/walk/Root.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/rules/Types.hpp"
#include "../../src/utility/IntegerTypes.hpp"

namespace dctl {
namespace walk {

struct Fixture
{
        Fixture()
        {
                root_.resize_hash(24);
        }

        ~Fixture()
        {
                root_.resize_hash(1);
        }

        BOOST_STATIC_CONSTANT(auto, R = 0);

        template<typename Position, std::size_t N>
        void run(Position const& p, NodeCount const (&/*leafs*/)[N])
        {/*
                for (auto it = std::begin(leafs); it != std::prev(std::end(leafs), R); ++it) {
                        root_.clear_hash();
                        auto const depth = std::distance(std::begin(leafs), it) + 1;
                        BOOST_CHECK_EQUAL(*it, root_.test(p, depth));
                }*/
                root_.perft(p, N);
        };

private:
        Root root_;
};

}       // namespace walk
}       // namespace dctl
