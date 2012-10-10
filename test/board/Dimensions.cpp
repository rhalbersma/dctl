#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/group/action.hpp"

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestDimensions)

typedef boost::mpl::vector<
        Dimensions< 4,  4>,
        Dimensions< 6,  6>,
        Dimensions< 8,  8>,
        Dimensions<10, 10>,
        Dimensions<10,  8, true>,
        Dimensions<11, 10, true>,
        Dimensions<12, 10, true>
> DimensionsSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, DimensionsSequence)
{
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C1 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C2 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C4 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
