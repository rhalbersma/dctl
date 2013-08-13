#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/traits.hpp>                // is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative

namespace dctl {
namespace angle {
namespace lazy {

BOOST_AUTO_TEST_SUITE(AngleTraits)

using OrthogonalAngles = boost::mpl::vector
<
        D000, D090, D180, D270
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsOrthogonal, T, OrthogonalAngles)
{
        BOOST_STATIC_ASSERT( is_orthogonal(T::value));
        BOOST_STATIC_ASSERT(!is_diagonal  (T::value));
}

using DiagonalAngles = boost::mpl::vector
<
        D045, D135, D225, D315
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDiagonal, T, DiagonalAngles)
{
        BOOST_STATIC_ASSERT( is_diagonal  (T::value));
        BOOST_STATIC_ASSERT(!is_orthogonal(T::value));
}

using UpAngles = boost::mpl::vector
<
        D045, D090, D135
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsUp, T, UpAngles)
{
        BOOST_STATIC_ASSERT( is_up  (T::value));
        BOOST_STATIC_ASSERT(!is_down(T::value));
}

using DownAngles = boost::mpl::vector
<
        D225, D270, D315
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDown, T, DownAngles)
{
        BOOST_STATIC_ASSERT( is_down(T::value));
        BOOST_STATIC_ASSERT(!is_up  (T::value));
}

using LeftAngles = boost::mpl::vector
<
        D135, D180, D225
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsLeft, T, LeftAngles)
{
        BOOST_STATIC_ASSERT( is_left (T::value));
        BOOST_STATIC_ASSERT(!is_right(T::value));
}

using RightAngles = boost::mpl::vector
<
        D315, D000, D045
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRight, T, RightAngles)
{
        BOOST_STATIC_ASSERT( is_right(T::value));
        BOOST_STATIC_ASSERT(!is_left (T::value));
}

using PositiveAngles = boost::mpl::vector
<
        D045, D090, D135, D180
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPositive, T, PositiveAngles)
{
        BOOST_STATIC_ASSERT( is_positive(T::value));
        BOOST_STATIC_ASSERT(!is_negative(T::value));
}

using NegativeAngles = boost::mpl::vector
<
        D225, D270, D315, D000
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsNegative, T, NegativeAngles)
{
        BOOST_STATIC_ASSERT( is_negative(T::value));
        BOOST_STATIC_ASSERT(!is_positive(T::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace lazy
}       // namespace angle
}       // namespace dctl
