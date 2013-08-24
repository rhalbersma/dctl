#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/grid/coordinates.hpp>           // Coordinates
#include <dctl/grid/coordinates/transform.hpp> // rotate
#include <dctl/grid/dimensions.hpp>            // Dimensions
#include <dctl/grid/edge.hpp>
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // C1, C2, C4

namespace dctl {
namespace grid {

BOOST_AUTO_TEST_SUITE(TestCoordinates)
/*
using Coord = Coordinates< 0, 0 >;
using CoordinatesSequence = boost::mpl::vector< Coord >;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, CoordinatesSequence)
{
        BOOST_CHECK(true);

        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C1 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C2 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C4 > ));

}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace grid
}       // namespace dctl
