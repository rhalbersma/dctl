#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include <boost/type_traits/is_same.hpp>
#include "../../../DCTL/src/board/Dimensions.h"
#include "../../../DCTL/src/board/Transform.h"

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestDimensions)

typedef boost::mpl::list<
        Dimensions< 4,  4>,
        Dimensions< 6,  6>,
        Dimensions< 8,  8>,
        Dimensions<10, 10>,
        Dimensions<10,  8, true>,
        Dimensions<11, 10, true>,
        Dimensions<12, 10, true>
> DimensionsList;

BOOST_AUTO_TEST_CASE_TEMPLATE(Inverse, T, DimensionsList)
{
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::R090>::type, Degrees::L090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::L090>::type, Degrees::R090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::D180>::type, Degrees::D180>::type>::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Square, T, DimensionsList)
{
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D180>::type, rotate<rotate<T, Degrees::L090>::type, Degrees::L090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D180>::type, rotate<rotate<T, Degrees::R090>::type, Degrees::R090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D000>::type, rotate<rotate<T, Degrees::D180>::type, Degrees::D180>::type>::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
