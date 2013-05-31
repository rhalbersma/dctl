#include <iterator>                             // back_inserter
#include <string>                               // string
#include <tuple>                                // make_tuple, tuple
#include <vector>                               // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <dctl/utility/variadic.hpp>            // cartesian_product

namespace dctl {
namespace variadic {

BOOST_AUTO_TEST_SUITE(UtilityVariadic)

BOOST_AUTO_TEST_CASE(CartesianProduct)
{
        bool b[] = { false, true };
        int i[] = { 0, 1 };
        std::string s[] = { "Hello", "World" };

        std::vector< std::tuple<bool, int, std::string> > cp = {
                std::make_tuple(false, 0, "Hello") ,
                std::make_tuple(false, 0, "World"),
                std::make_tuple(false, 1, "Hello"),
                std::make_tuple(false, 1, "World"),
                std::make_tuple(true,  0, "Hello"),
                std::make_tuple(true,  0, "World"),
                std::make_tuple(true,  1, "Hello"),
                std::make_tuple(true,  1, "World")
        };

        std::vector< std::tuple<bool, int, std::string> > result;
        cartesian_product(
                std::back_inserter(result),
                std::make_pair(std::begin(b), std::end(b)),
                std::make_pair(std::begin(i), std::end(i)),
                std::make_pair(std::begin(s), std::end(s))
        );

        // cannot use BOOST_CHECK_EQUAL_COLLECTIONS because std::tuple has no operator<<
        BOOST_CHECK(result == cp);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace variadic
}       // namespace dctl
