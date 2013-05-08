#include <test/config.hpp>        // SUCCESSOR_TEST

#if SUCCESSOR_TEST == 1

#include <string>                       // string
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <test/successor/fixture.hpp>   // run
#include <dctl/board/types.hpp>         // Roman
#include <dctl/rules/variants.hpp>         // Italian

namespace dctl {
namespace successor {

typedef Fixture<rules::Italian, board::Roman> FixtureItalian;

BOOST_AUTO_TEST_SUITE(TestItalian)

// Positions from the official Italian rules:
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

BOOST_FIXTURE_TEST_CASE(pawnJumpDirections, FixtureItalian)
{
        // Art. 5.6 (pawn jump directions)
        auto const FEN = "W:W30:B27";
        std::string const legal[] = { "30x23" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(pawnJumpContinuation, FixtureItalian)
{
        // Art. 5.7 (pawn jump continuation)
        auto const FEN = "W:W31:B12,20,28";
        std::string const legal[] = { "31x8" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(kingJumpDirection, FixtureItalian)
{
        // Art. 5.8 (king jump directions)
        auto const FEN = "W:WK22:B18,19,27";
        std::string const legal[] = { "22x13", "22x15", "22x31" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(kingJumpContinuation, FixtureItalian)
{
        // Art. 5.9 (king jump continuation)
        auto const FEN = "W:WK27:B5,6,13,15,21,23";
        std::string const legal[] = { "27x25" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(jumpMostPieces, FixtureItalian)
{
        // Art. 6.6 (jump most pieces)
        auto const FEN = "W:WK21,31:B10,18,27";
        std::string const legal[] = { "31x6" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(jumpWithKing, FixtureItalian)
{
        // Art. 6.7 (jump with king)
        auto const FEN = "W:WK23,31:B19,26,27";
        std::string const legal[] = { "23x21" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(jumpMostKings, FixtureItalian)
{
        // Art. 6.8 (jump most kings)
        auto const FEN = "W:WK23:B19,K28";
        std::string const legal[] = { "23x32" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(jumpFirstKing, FixtureItalian)
{
        // Art. 6.9 (jump first king)
        auto const FEN = "W:WK30:B10,12,18,K20,K26,27";
        std::string const legal[] = { "30x5" };
        run(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(equivalentJump, FixtureItalian)
{
        // Art. 6.10 (equivalent jump)
        auto const FEN = "W:WK31:B18,20,K27,K28";
        std::string const legal[] = { "31x13", "31x15" };
        run(FEN, legal);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl

#endif
