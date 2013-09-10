#include <type_traits>                          // is_same
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform.hpp>              // transform
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315, D360
#include <dctl/angle/transform.hpp>             // rotate
#include <dctl/grid/dimensions.hpp>             // Dimensions
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size

namespace dctl {
namespace grid {

BOOST_AUTO_TEST_SUITE(TestDimensions)

using DimensionsSequence = boost::mpl::vector<
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
        using type = Grid<Dim, 2> ;
};

using GridSequence = boost::mpl::transform<
        DimensionsSequence,
        make_grid< boost::mpl::_1 >
>::type;

BOOST_AUTO_TEST_CASE_TEMPLATE(ShiftSizeSymmetry, T, GridSequence)
{
        for (auto alpha = angle::D000; alpha != angle::D360; alpha += angle::D045)
                BOOST_CHECK_EQUAL(shift_size<T>(alpha), shift_size<T>(angle::rotate(alpha, angle::D180)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace grid
}       // namespace dctl
