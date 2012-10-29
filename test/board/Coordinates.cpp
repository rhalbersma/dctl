#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/Coordinates.hpp>           // Coordinates
#include <dctl/board/coordinates/transform.hpp> // rotate
#include <dctl/board/Dimensions.hpp>            // Dimensions
#include <dctl/board/Grid.hpp>                  // Grid
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // C1, C2, C4

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestCoordinates)

typedef Grid< Dimensions< 4,  4>, boost::mpl::int_<2> > G;

typedef Coordinates< G, 0, 0 > Coord;

typedef boost::mpl::vector<
        Coord
> CoordinatesSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, CoordinatesSequence)
{
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C1 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C2 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C4 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
