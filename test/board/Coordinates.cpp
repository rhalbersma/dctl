#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/int_fwd.hpp>                // int_
#include <boost/mpl/vector.hpp>                 // vector

#include "../../src/board/Coordinates.hpp"
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Grid.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/group/action.hpp"

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
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C1 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C2 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C4 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
