#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector

#include "../../src/board/Coordinates.hpp"
#include "../../src/board/Dimensions.hpp"
#include "../../src/board/Grid.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/group/action.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestCoordinates)

typedef boost::mpl::vector<
        board::Dimensions< 4,  4>,
        board::Dimensions< 6,  6>,
        board::Dimensions< 8,  8>,
        board::Dimensions<10, 10>,
        board::Dimensions<10,  8, true>,
        board::Dimensions<11, 10, true>,
        board::Dimensions<12, 10, true>
> DimensionsSequence;

typedef board::Grid< board::Dimensions< 4,  4>, 2 > G;

typedef boost::mpl::vector<
        board::Coordinates<G, 0, 0>
> CoordinatesSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, CoordinatesSequence)
{
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C1 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C2 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, angle::C4 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
