#include <dctl/board/angle.hpp>                 // angle, _deg, rotate
#include <board/sequence.hpp>                   // micro, mini, checkers, roman, spantsireti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, compact_10_12, compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <algorithm>                            // all_of
#include <vector>                               // vector

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(GridShiftSize)

BOOST_AUTO_TEST_CASE_TEMPLATE(RotateAngle180SymmetryForAllDirections, T, BoardSequence)
{
        auto const directions = std::vector<angle>
        {
                   0_deg,  45_deg,  90_deg, 135_deg,
                 180_deg, 225_deg, 270_deg, 315_deg
        };

        BOOST_CHECK(
                std::all_of(directions.cbegin(), directions.cend(), [](auto dir) {
                        return T::shift_size(dir) == T::shift_size(rotate(dir, 180_deg));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
