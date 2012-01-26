#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>

#include <boost/mpl/list.hpp>                           // boost::mpl::list

#include "Transform.hpp"
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Group.hpp"

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

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, DimensionsList)
{
        is_right_action<T, group::C1>()();
        is_right_action<T, group::C2>()();
        is_right_action<T, group::C4>()();
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
