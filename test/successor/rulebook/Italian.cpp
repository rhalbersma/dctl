#include <boost/test/unit_test.hpp>
#include "Fixture.hpp"
#include "../../test_config.hpp"
#include "../../../src/board/Types.hpp"
#include "../../../src/rules/Types.hpp"

#if SUCCESSOR_TEST == 1

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(TestItalian)

typedef Fixture<rules::Italian, board::Roman> FixtureItalian;

// Positions from the official Italian rules:
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

BOOST_FIXTURE_TEST_CASE(TestPawnCaptureDirections, FixtureItalian)
{
        // Art. 5.6  (pawn capture directions)
        auto const FEN = "W:W30:B27";
        std::string const legal[] = { "30x23" };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestPawnCaptureContinuation, FixtureItalian)
{
        // Art. 5.7  (pawn capture continuation)
        auto const FEN = "W:W31:B12,20,28";
        std::string const legal[] = { "31x8 " };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestKingCaptureDirection, FixtureItalian)
{
        // Art. 5.8  (king capture directions)
        auto const FEN = "W:WK22:B18,19,27";
        std::string const legal[] = { "22x13", "22x15", "22x31" };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestKingCaptureContinuation, FixtureItalian)
{
        // Art. 5.9  (king capture continuation)
        auto const FEN = "W:WK27:B5,6,13,15,21,23";
        std::string const legal[] = { "27x25" };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestCaptureMostPieces, FixtureItalian)
{
        // Art. 6.6  (capture most number of pieces)
        auto const FEN = "W:WK21,31:B10,18,27";
        std::string const legal[] = { "31x6 " };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestCaptureWithKing, FixtureItalian)
{
        // Art. 6.7  (capture with king)
        auto const FEN = "W:WK23,31:B19,26,27";
        std::string const legal[] = { "23x21" };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestCaptureMostKings, FixtureItalian)
{
        // Art. 6.8  (capture most kings)
        auto const FEN = "W:WK23:B19,K28";
        std::string const legal[] = { "23x32" };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestCaptureFirstKings, FixtureItalian)
{
        // Art. 6.9  (capture first kings)
        auto const FEN = "W:WK30:B10,12,18,K20,K26,27";
        std::string const legal[] = { "30x5 " };                
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(TestEquivalentCapture, FixtureItalian)
{
        // Art. 6.10 (equivalent capture)
        auto const FEN = "W:WK31:B18,20,K27,K28";
        std::string const legal[] = { "31x13", "31x15" };                
        run(FEN, legal);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl

#endif
