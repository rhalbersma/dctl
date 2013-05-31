#include <string>                       // string
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <test/successor/fixture.hpp>   // run
#include <dctl/board/types.hpp>         // International
#include <dctl/rules/variants.hpp>      // International

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SucessorRulebookInternational)

// Positions from the official International rules (Italian translation):
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_II.pdf

typedef Fixture<rules::International, board::International> F;

BOOST_FIXTURE_TEST_CASE(KingMoveRange, F)
{
        // Art. 3.9 (king move range)
        auto const FEN = "W:WK23";
        std::string const legal[] = {
                "23-18", "23-12", "23-7", "23-1",
                "23-19", "23-14", "23-10", "23-5",
                "23-28", "23-32", "23-37", "23-41", "23-46",
                "23-29", "23-34", "23-40", "23-45"
        };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(PawnJumpDirections, F)
{
        // Art. 4.2 (pawn jump directions)
        auto const FEN = "W:W35:B30,K40";
        std::string const legal[] = { "35x24", "35x44" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(KingJumpRange, F)
{
        // Art. 4.3 (king jump range)
        auto const FEN = "W:WK41:B23";
        std::string const legal[] = { "41x19", "41x14", "41x10", "41x5" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)
{
        // Art. 4.5 (pawn jump continuation)
        auto const FEN = "W:W47:B13,14,22,24,31,34,K41,44";
        std::string const legal[] = { "47x49" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)
{
        // Art. 4.6 (king jump continuation)
        auto const FEN = "W:WK1:B7,9,17,19,20,30,31,33,43,44";
        std::string const legal[] = { "1x15" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(PhaseCapture, F)
{
        // Art. 4.8 (capture phase NOT en-passant)
        auto const FEN = "B:W27,28,38,39,42:BK25";
        std::string const legal[] = { "25x33" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(JumpMostPieces, F)
{
        // Art. 4.13 (jump most pieces)
        auto const FEN = "W:WK48:B7,8,31,34,K42,44";
        std::string const legal[] = { "48x50" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(JumpMostKings, F)
{
        // Art. 4.14 (jump most kings NOT applicable)
        auto const FEN = "W:W26:B12,K21,31,32";
        std::string const legal[] = { "26x8", "26x28" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(PhasePromotion, F)
{
        // Art. 4.15 (promotion phase NOT en-passant)
        auto const FEN = "W:W15:B9,10";
        std::string const legal[] = { "15x13" };
        run(FEN, legal);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
