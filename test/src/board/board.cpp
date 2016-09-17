#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsiretti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact_10_12, Compact_12_10, Rectangular<12, 10>, Canadian, SriLankan, Dumm
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(Bit2SquareIsInverseSquare2Bit, T, BoardSequence)
{
        BOOST_CHECK(
                boost::algorithm::all_of(T::squares(), [](auto const sq) {
                        return T::square_from_bit(T::bit_from_square(sq)) == sq;
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
