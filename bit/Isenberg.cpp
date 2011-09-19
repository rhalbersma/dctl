#include <boost/test/unit_test.hpp> 
#include "../../src/bit/Isenberg.hpp"
#include "../../src/utility/IntegerTypes.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestIsenberg)

BOOST_AUTO_TEST_CASE(Generate)
{
        typedef uint16_t T;
        findDeBruijn<T>(T(0), T(0), 12, 0);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
