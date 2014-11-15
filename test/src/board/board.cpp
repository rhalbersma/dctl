#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsireti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact1012, Compact120, Board<12, 10>, Canadian, SriLankan, Dumm
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(Bit2SquareIsInverseSquare2Bit, T, BoardSequence)
{
        for (auto&& xsq : T::squares()) {
                auto sq = static_cast<std::size_t>(xsq);
                BOOST_CHECK_EQUAL(T::square_from_bit(T::bit_from_square(sq)), sq);
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
