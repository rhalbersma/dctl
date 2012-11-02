#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE
#include <dctl/board/dimensions.hpp>            // Dimensions
#include <dctl/board/dimensions/transform.hpp>  // rotate
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // C1, C2, C4

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
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C1 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C2 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C4 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
