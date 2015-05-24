#include <dctl/angle.hpp>                       // Angle, _deg, rotate
#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsireti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact1012, Compact120, Board<12, 10>, Canadian, SriLankan, Dumm
#include <range/v3/all.hpp>                     // all_of
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <vector>                               // vector

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(GridShiftSize)

BOOST_AUTO_TEST_CASE_TEMPLATE(RotateAngle180SymmetryForAllDirections, T, BoardSequence)
{
        auto const directions = std::vector<Angle>
        {
                   0_deg,  45_deg,  90_deg, 135_deg,
                 180_deg, 225_deg, 270_deg, 315_deg
        };

        BOOST_CHECK(
                ranges::all_of(directions, [](auto dir) {
                        return T::shift_size(dir) == T::shift_size(rotate(dir, 180_deg));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
