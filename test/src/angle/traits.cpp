#include <dctl/angle.hpp>               // _deg, is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleTraits)

BOOST_AUTO_TEST_CASE(OrthogonalIsNotDiagonal)
{
        for (auto&& a : { 0_deg, 90_deg, 180_deg, 270_deg }) {
                BOOST_CHECK( is_orthogonal(a));
                BOOST_CHECK(!is_diagonal  (a));
        }
}

BOOST_AUTO_TEST_CASE(DiagonalIsNotOrthogonal)
{
        for (auto&& a : { 45_deg, 135_deg, 225_deg, 315_deg }) {
                BOOST_CHECK( is_diagonal  (a));
                BOOST_CHECK(!is_orthogonal(a));
        }
}

BOOST_AUTO_TEST_CASE(UpIsNotDown)
{
        for (auto&& a : { 45_deg, 90_deg, 135_deg }) {
                BOOST_CHECK( is_up  (a));
                BOOST_CHECK(!is_down(a));
        }
}

BOOST_AUTO_TEST_CASE(DownIsNotUp)
{
        for (auto&& a : { 225_deg, 270_deg, 315_deg }) {
                BOOST_CHECK( is_down(a));
                BOOST_CHECK(!is_up  (a));
        }
}

BOOST_AUTO_TEST_CASE(LeftIsNotRight)
{
        for (auto&& a : { 135_deg, 180_deg, 225_deg }) {
                BOOST_CHECK( is_left (a));
                BOOST_CHECK(!is_right(a));
        }
}

BOOST_AUTO_TEST_CASE(RightIsNotLeft)
{
        for (auto a : { 315_deg, 0_deg, 45_deg }) {
                BOOST_CHECK( is_right(a));
                BOOST_CHECK(!is_left (a));
        }
}

BOOST_AUTO_TEST_CASE(PositiveIsNotNegative)
{
        for (auto&& a : { 45_deg, 90_deg, 135_deg, 180_deg }) {
                BOOST_CHECK( is_positive(a));
                BOOST_CHECK(!is_negative(a));
        }
}

BOOST_AUTO_TEST_CASE(NegativeIsNotPositive)
{
        for (auto a : { 225_deg, 270_deg, 315_deg, 0_deg }) {
                BOOST_CHECK( is_negative(a));
                BOOST_CHECK(!is_positive(a));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
