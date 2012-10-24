#pragma once
#include <cstddef>                              // size_t
#include <iterator>                             // begin, end
#include <boost/config.hpp>                     // BOOST_STATIC_CONSTANT
#include <boost/test/unit_test.hpp>             // BOOST_CHECK_EQUAL
#include <dctl/walk/Root.hpp>                   // Root
#include <dctl/utility/IntegerTypes.hpp>        // NodeCount

namespace dctl {
namespace walk {

struct Fixture
{
public:
        Fixture()
        {
                root_.resize_hash(25);
        }

        ~Fixture()
        {
                root_.resize_hash(1);
        }

        BOOST_STATIC_CONSTANT(auto, R = 0);

        template<typename Position, std::size_t N>
        void run(Position const& p, NodeCount const (&leafs)[N])
        {
                for (auto it = std::begin(leafs); it != std::prev(std::end(leafs), R); ++it) {
                        root_.clear_hash();
                        auto const depth = std::distance(std::begin(leafs), it) + 1;
                        BOOST_CHECK_EQUAL(*it, root_.test(p, depth));
                }
        }

private:
        Root root_;
};

}       // namespace walk
}       // namespace dctl
