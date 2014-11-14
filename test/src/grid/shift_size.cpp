#include <dctl/angle.hpp>                       // Angle, _deg, rotate
#include <dctl/board/types.hpp>                 // Micro, Mini, Checkers, International, Roman, Frisian, Spantsireti, Ktar11, Ktar12
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>           // _1
#include <boost/mpl/transform.hpp>              // transform
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <algorithm>                            // all_of
#include <iterator>                             // begin, end

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(GridShiftSize)

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        International,
        Canadian,
        Dumm,
        Roman,
        Frisian,
        SriLankan,
        Spantsireti,
        Ktar<10, 11>,
        Ktar<10, 12>
>;

Angle const directions[] =
{
           0_deg,  45_deg,  90_deg, 135_deg,
         180_deg, 225_deg, 270_deg, 315_deg
};

BOOST_AUTO_TEST_CASE_TEMPLATE(RotateAngle180SymmetryForAllDirections, T, BoardSequence)
{
        using std::begin; using std::end;
        BOOST_CHECK(
                std::all_of(begin(directions), end(directions), [](auto direction) {
                        return T::shift_size(direction) == T::shift_size(rotate(direction, 180_deg));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
