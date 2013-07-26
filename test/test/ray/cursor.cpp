#include <iostream>
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/ray/cursor.hpp>

namespace dctl {
namespace ray {

BOOST_AUTO_TEST_SUITE(RayCursor)

BOOST_AUTO_TEST_CASE(Members)
{
        auto c = StridedCursor<2>(0);
        for (auto i = 0; i < 5; ++i) {
                ++c;
                std::cout << c << std::endl;
        }
        std::cout << (StridedCursor<2>(3) - StridedCursor<2>(5));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace ray
}       // namespace dctl
