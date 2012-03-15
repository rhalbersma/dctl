#include <boost/test/unit_test.hpp>
#include "../../src/bit/DeBruijn.hpp"
#include "../../src/utility/IntegerTypes.hpp"

namespace dctl {
namespace bit {
namespace debruijn {

BOOST_AUTO_TEST_SUITE(TestIsenberg)

BOOST_AUTO_TEST_CASE(Generate)
{
        std::cout << std::dec << bit::debruijn::Graph<2>::Eulerian() << '\n';
        std::cout << std::dec << bit::debruijn::Graph<3>::Eulerian() << '\n';
        std::cout << std::dec << bit::debruijn::Graph<4>::Eulerian() << '\n';
        //std::cout << std::dec << bit::debruijn::Graph<5>::Eulerian() << '\n';

}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace debruijn
}       // namespace bit
}       // namespace dctl
