#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>                           // boost::mpl::list
#include <boost/mpl/transform.hpp>
#include <boost/mpl/lambda.hpp>

#include "../../src/board/Coordinates.hpp"
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Grid.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/board/Group.hpp"

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(TestCoordinates)

namespace mpl = boost::mpl;

typedef mpl::list<
        board::Dimensions< 4,  4>,
        board::Dimensions< 6,  6>,
        board::Dimensions< 8,  8>,
        board::Dimensions<10, 10>,
        board::Dimensions<10,  8, true>,
        board::Dimensions<11, 10, true>,
        board::Dimensions<12, 10, true>
> DimensionsList;

typedef board::Grid< board::Dimensions< 4,  4>, 2 > G;

typedef boost::mpl::list<
        board::Coordinates<G, 0, 0>
> CoordinatesList;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, CoordinatesList)
{
        BOOST_CHECK((is_right_action< T, C1 >::value));
        BOOST_CHECK((is_right_action< T, C2 >::value));
        BOOST_CHECK((is_right_action< T, C4 >::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
