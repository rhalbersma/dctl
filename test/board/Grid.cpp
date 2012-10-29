#include <type_traits>                          // is_same
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform.hpp>              // transform
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/Dimensions.hpp>            // Dimensions
#include <dctl/board/Grid.hpp>                  // Grid

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

template<typename Dim>
struct make_grid
{
        typedef Grid<Dim, boost::mpl::int_<2> > type;
};

typedef boost::mpl::transform< 
        DimensionsSequence, 
        make_grid< boost::mpl::_1 >
>::type GridSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(ShiftSizeSymmetry, T, GridSequence)
{
        BOOST_MPL_ASSERT(( std::is_same<typename T::right   , typename T::left      > ));
        BOOST_MPL_ASSERT(( std::is_same<typename T::right_up, typename T::left_down > ));
        BOOST_MPL_ASSERT(( std::is_same<typename T::up      , typename T::down      > ));
        BOOST_MPL_ASSERT(( std::is_same<typename T::left_up , typename T::right_down> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
