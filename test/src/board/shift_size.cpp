#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsiretti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact_10_12, Compact_12_10, Rectangular<12, 10>, Canadian, SriLankan, Dumm
#include <dctl/board/angle.hpp>                 // angle, reverse
#include <dctl/board/shift.hpp>                 // shift_size
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <vector>                               // vector

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(GridShiftSize)

BOOST_AUTO_TEST_CASE_TEMPLATE(ReverseangleSymmetryForAllDirections, T, BoardSequence)
{
        auto const directions = std::vector<angle>
        {
                   0_deg,  45_deg,  90_deg, 135_deg,
                 180_deg, 225_deg, 270_deg, 315_deg
        };

        BOOST_CHECK(
                boost::algorithm::all_of(directions, [](auto dir) {
                        return shift_size<T>{}(dir) == shift_size<T>{}(reverse(dir));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
