#include <type_traits>                          // is_same
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform.hpp>              // transform
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/dimensions.hpp>            // Dimensions
#include <dctl/board/edge.hpp>
#include <dctl/board/grid.hpp>                  // Grid

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestDimensions)

using DimensionsSequence = boost::mpl::vector
<
        Dimensions< 4,  4>,
        Dimensions< 6,  6>,
        Dimensions< 8,  8>,
        Dimensions<10, 10>,
        Dimensions<10,  8, true>,
        Dimensions<11, 10, true>,
        Dimensions<12, 10, true>
>;

template<class Dim>
struct make_grid
{
        using type = Grid<Dim, DoubleColumnEdge> ;
};

using GridSequence = boost::mpl::transform
<
        DimensionsSequence, 
        make_grid< boost::mpl::_1 >
>::type;

BOOST_AUTO_TEST_CASE_TEMPLATE(ShiftSizeSymmetry, T, GridSequence)
{
        BOOST_STATIC_ASSERT( T::right    == T::left       );
        BOOST_STATIC_ASSERT( T::right_up == T::left_down  );
        BOOST_STATIC_ASSERT( T::up       == T::down       );
        BOOST_STATIC_ASSERT( T::left_up  == T::right_down );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
