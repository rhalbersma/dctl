#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include "../../src/board/Traits.hpp"
#include "../../src/board/Angle.hpp"
#include "../../src/board/Degrees.hpp"

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestTraits)

typedef boost::mpl::vector<
        angle<degrees::D000>, 
        angle<degrees::D090>, 
        angle<degrees::D180>, 
        angle<degrees::D270>
> OrthogonalAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsOrthogonal, T, OrthogonalAngles)
{
        BOOST_CHECK( is_orthogonal<T>::value);
        BOOST_CHECK(!is_diagonal  <T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D045>, 
        angle<degrees::D135>, 
        angle<degrees::D225>, 
        angle<degrees::D315>
> DiagonalAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDiagonal, T, DiagonalAngles)
{
        BOOST_CHECK( is_diagonal  <T>::value);
        BOOST_CHECK(!is_orthogonal<T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D045>, 
        angle<degrees::D090>, 
        angle<degrees::D135>
> UpAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsUp, T, UpAngles)
{
        BOOST_CHECK( is_up  <T>::value);
        BOOST_CHECK(!is_down<T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D225>, 
        angle<degrees::D270>, 
        angle<degrees::D315>
> DownAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDown, T, DownAngles)
{
        BOOST_CHECK( is_down<T>::value);
        BOOST_CHECK(!is_up  <T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D135>, 
        angle<degrees::D180>, 
        angle<degrees::D225>
> LeftAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsLeft, T, LeftAngles)
{
        BOOST_CHECK( is_left <T>::value);
        BOOST_CHECK(!is_right<T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D315>, 
        angle<degrees::D000>, 
        angle<degrees::D045>
> RightAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRight, T, RightAngles)
{
        BOOST_CHECK( is_right<T>::value);
        BOOST_CHECK(!is_left <T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D045>, 
        angle<degrees::D090>, 
        angle<degrees::D135>,
        angle<degrees::D180>
> PositiveAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPositive, T, PositiveAngles)
{
        BOOST_CHECK( is_positive<T>::value);
        BOOST_CHECK(!is_negative<T>::value);
}

typedef boost::mpl::vector<
        angle<degrees::D225>, 
        angle<degrees::D270>, 
        angle<degrees::D315>,
        angle<degrees::D000>
> NegativeAngles;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsNegative, T, NegativeAngles)
{
        BOOST_CHECK( is_negative<T>::value);
        BOOST_CHECK(!is_positive<T>::value);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
