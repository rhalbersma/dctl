#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/traits.hpp>                // is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleTraits)

BOOST_AUTO_TEST_CASE(OnlyOrthogonalIsOrthogonal)
{
        for (auto const& a: { D000, D090, D180, D270 }) {
                BOOST_CHECK( is_orthogonal(a));
                BOOST_CHECK(!is_diagonal  (a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyDiagonalIsDiagonal)
{
        for (auto const& a: { D045, D135, D225, D315 }) {
                BOOST_CHECK( is_diagonal  (a));
                BOOST_CHECK(!is_orthogonal(a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyUpIsUp)
{
        for (auto const& a: { D045, D090, D135 }) {
                BOOST_CHECK( is_up  (a));
                BOOST_CHECK(!is_down(a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyDownIsDown)
{
        for (auto const& a: { D225, D270, D315 }) {
                BOOST_CHECK( is_down(a));
                BOOST_CHECK(!is_up  (a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyLeftIsLeft)
{
        for (auto const& a: { D135, D180, D225 }) {
                BOOST_CHECK( is_left (a));
                BOOST_CHECK(!is_right(a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyRightIsRight)
{
        for (auto const& a: { D315, D000, D045 }) {
                BOOST_CHECK( is_right(a));
                BOOST_CHECK(!is_left (a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyPositiveIsPositive)
{
        for (auto const& a: { D045, D090, D135, D180 }) {
                BOOST_CHECK( is_positive(a));
                BOOST_CHECK(!is_negative(a));
        }
}

BOOST_AUTO_TEST_CASE(OnlyNegativeIsNegative)
{
        for (auto const& a: { D225, D270, D315, D000 }) {
                BOOST_CHECK( is_negative(a));
                BOOST_CHECK(!is_positive(a));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
