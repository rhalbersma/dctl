#include <algorithm>                            // all_of
#include <iterator>                             // begin, end
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform.hpp>              // transform
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <dctl/angle/angle.hpp>                 // Angle
#include <dctl/angle/degrees.hpp>               // _deg
#include <dctl/angle/transform.hpp>             // rotate
#include <dctl/board/types.hpp>                 // Micro, Mini, Checkers, International, Roman, Frisian, Spantsireti, Ktar11, Ktar12
#include <dctl/grid/shift_size.hpp>             // shift_size

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(GridShiftSize)

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        International,
        Roman,
        Frisian,
        Spantsireti,
        Ktar11,
        Ktar12
>;

template<class Board>
struct make_grid
{
        using type = typename Board::InternalGrid;
};

using GridSequence = boost::mpl::transform<
        BoardSequence,
        make_grid< boost::mpl::_1 >
>::type;

Angle const directions[] =
{
           0_deg,  45_deg,  90_deg, 135_deg,
         180_deg, 225_deg, 270_deg, 315_deg
};

BOOST_AUTO_TEST_CASE_TEMPLATE(RotateAngle180SymmetryForAllDirections, T, GridSequence)
{
        BOOST_CHECK(
                std::all_of(std::begin(directions), std::end(directions), [](auto direction) {
                        return grid::shift_size<T>(direction) == grid::shift_size<T>(rotate(direction, 180_deg));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
