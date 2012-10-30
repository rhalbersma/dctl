#pragma once
#include <cstddef>                              // size_t
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

        template<typename Position, std::size_t N>
        void run(Position const& p, NodeCount const (&leafs)[N])
        {
                auto depth = 0;
                for (auto const& node_count: leafs) {
                        root_.clear_hash(); 
                        BOOST_CHECK_EQUAL(node_count, root_.test(p, ++depth));
                }
        }

private:
        Root root_;
};

}       // namespace walk
}       // namespace dctl
