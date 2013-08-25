#include <utility>                              // make_pair
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <dctl/utility/abs.hpp>                 // is_negative, is_positive, is_bounded, abs, abs_remainder

namespace dctl {
namespace util {

BOOST_AUTO_TEST_SUITE(UtilityAbs)

BOOST_AUTO_TEST_CASE(OnlyNegativeIsNegative)
{
        BOOST_CHECK( is_negative(-1));
        BOOST_CHECK(!is_negative( 0));
        BOOST_CHECK(!is_negative( 1));
}

BOOST_AUTO_TEST_CASE(OnlyPositiveIsPositive)
{
        BOOST_CHECK(!is_positive(-1));
        BOOST_CHECK(!is_positive( 0));
        BOOST_CHECK( is_positive( 1));
}

BOOST_AUTO_TEST_CASE(OnlyHalfOpenRangeIsBounded)
{
        auto const range = std::make_pair(0, 2);
        BOOST_CHECK(!is_bounded(range.first  - 1, range));
        BOOST_CHECK( is_bounded(range.first     , range));
        BOOST_CHECK( is_bounded(range.second - 1, range));
        BOOST_CHECK(!is_bounded(range.second    , range));
}

BOOST_AUTO_TEST_CASE(AbsIsNotNegative)
{
        BOOST_CHECK(!is_negative(util::abs(-1)));
        BOOST_CHECK(!is_negative(util::abs( 0)));
        BOOST_CHECK(!is_negative(util::abs( 1)));
}

BOOST_AUTO_TEST_CASE(AbsRemainderIsBoundedByAbsDivisor)
{
        BOOST_CHECK(is_bounded(abs_remainder(-2, 2), {0, util::abs( 2)}));
        BOOST_CHECK(is_bounded(abs_remainder(-1, 2), {0, util::abs( 2)}));
        BOOST_CHECK(is_bounded(abs_remainder(-0, 2), {0, util::abs( 2)}));

        BOOST_CHECK(is_bounded(abs_remainder( 0, 2), {0, util::abs( 2)}));
        BOOST_CHECK(is_bounded(abs_remainder( 1, 2), {0, util::abs( 2)}));
        BOOST_CHECK(is_bounded(abs_remainder( 2, 2), {0, util::abs( 2)}));

        BOOST_CHECK(is_bounded(abs_remainder(-2,-2), {0, util::abs(-2)}));
        BOOST_CHECK(is_bounded(abs_remainder(-1,-2), {0, util::abs(-2)}));
        BOOST_CHECK(is_bounded(abs_remainder(-0,-2), {0, util::abs(-2)}));

        BOOST_CHECK(is_bounded(abs_remainder( 0,-2), {0, util::abs(-2)}));
        BOOST_CHECK(is_bounded(abs_remainder( 1,-2), {0, util::abs(-2)}));
        BOOST_CHECK(is_bounded(abs_remainder( 2,-2), {0, util::abs(-2)}));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace util
}       // namespace dctl
