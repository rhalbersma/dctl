#include <utility>                              // make_pair
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <dctl/utility/range.hpp>               // is_element

namespace dctl {
namespace util {

BOOST_AUTO_TEST_SUITE(UtilityRange)

BOOST_AUTO_TEST_CASE(IncludesFirstAndExcludesLast)
{
        auto const range = std::make_pair(0, 2);
        BOOST_CHECK( is_element(range.first , range));
        BOOST_CHECK(!is_element(range.second, range));
}

BOOST_AUTO_TEST_CASE(ExcludesBeforeFirstAndIncludesBeforeLast)
{
        auto const range = std::make_pair(0, 2);
        BOOST_CHECK(!is_element(range.first  - 1, range));
        BOOST_CHECK( is_element(range.second - 1, range));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace util
}       // namespace dctl
