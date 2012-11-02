#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/traits.hpp>                // is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative

namespace dctl {
namespace angle {
namespace lazy {

BOOST_AUTO_TEST_SUITE(TestTraits)

typedef boost::mpl::vector<
        D000, D090, D180, D270
> OrthogonalAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsOrthogonal, T, OrthogonalAngles)
{
        BOOST_MPL_ASSERT    (( is_orthogonal<T> ));
        BOOST_MPL_ASSERT_NOT(( is_diagonal  <T> ));
}

typedef boost::mpl::vector<
        D045, D135, D225, D315
> DiagonalAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDiagonal, T, DiagonalAngles)
{
        BOOST_MPL_ASSERT    (( is_diagonal  <T> ));
        BOOST_MPL_ASSERT_NOT(( is_orthogonal<T> ));
}

typedef boost::mpl::vector<
        D045, D090, D135
> UpAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsUp, T, UpAngles)
{
        BOOST_MPL_ASSERT    (( is_up  <T> ));
        BOOST_MPL_ASSERT_NOT(( is_down<T> ));
}

typedef boost::mpl::vector<
        D225, D270, D315
> DownAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDown, T, DownAngles)
{
        BOOST_MPL_ASSERT    (( is_down<T> ));
        BOOST_MPL_ASSERT_NOT(( is_up  <T> ));
}

typedef boost::mpl::vector<
        D135, D180, D225
> LeftAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsLeft, T, LeftAngles)
{
        BOOST_MPL_ASSERT    (( is_left <T> ));
        BOOST_MPL_ASSERT_NOT(( is_right<T> ));
}

typedef boost::mpl::vector<
        D315, D000, D045
> RightAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRight, T, RightAngles)
{
        BOOST_MPL_ASSERT    (( is_right<T> ));
        BOOST_MPL_ASSERT_NOT(( is_left <T> ));
}

typedef boost::mpl::vector<
        D045, D090, D135, D180
> PositiveAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPositive, T, PositiveAngles)
{
        BOOST_MPL_ASSERT    (( is_positive<T> ));
        BOOST_MPL_ASSERT_NOT(( is_negative<T> ));
}

typedef boost::mpl::vector<
        D225, D270, D315, D000
> NegativeAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsNegative, T, NegativeAngles)
{
        BOOST_MPL_ASSERT    (( is_negative<T> ));
        BOOST_MPL_ASSERT_NOT(( is_positive<T> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace lazy
}       // namespace angle
}       // namespace dctl
