#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/group/action.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestDimensions)

typedef boost::mpl::vector<
        board::Dimensions< 4,  4>,
        board::Dimensions< 6,  6>,
        board::Dimensions< 8,  8>,
        board::Dimensions<10, 10>,
        board::Dimensions<10,  8, true>,
        board::Dimensions<11, 10, true>,
        board::Dimensions<12, 10, true>
> DimensionsSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, DimensionsSequence)
{
        BOOST_CHECK((group::is_right_action< T, C1 >::value));
        BOOST_CHECK((group::is_right_action< T, C2 >::value));
        BOOST_CHECK((group::is_right_action< T, C4 >::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
