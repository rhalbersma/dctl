#include <successor/fixture.hpp>        // run
#include <dctl/board/types.hpp>         // Roman
#include <dctl/rules.hpp>               // Italian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string
#include <vector>                       // vector

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorRulebookItalian)

// Positions from the official Italian rules:
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

using F = Fixture<italian::Rules, board::Roman>;

BOOST_FIXTURE_TEST_CASE(PawnJumpDirections, F)
{
        // Art. 5.6 (pawn jump directions)
        auto const FEN = "W:W30:B27";
        auto const legal = std::vector<std::string>{ "30x23" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)
{
        // Art. 5.7 (pawn jump continuation)
        auto const FEN = "W:W31:B12,20,28";
        auto const legal = std::vector<std::string>{ "31x08" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(KingJumpDirection, F)
{
        // Art. 5.8 (king jump directions)
        auto const FEN = "W:WK22:B18,19,27";
        auto const legal = std::vector<std::string>{ "22x13", "22x15", "22x31" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)
{
        // Art. 5.9 (king jump continuation)
        auto const FEN = "W:WK27:B5,6,13,15,21,23";
        auto const legal = std::vector<std::string>{ "27x25" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(JumpMostPieces, F)
{
        // Art. 6.6 (jump most pieces)
        auto const FEN = "W:WK21,31:B10,18,27";
        auto const legal = std::vector<std::string>{ "31x06" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(JumpWithKing, F)
{
        // Art. 6.7 (jump with king)
        auto const FEN = "W:WK23,31:B19,26,27";
        auto const legal = std::vector<std::string>{ "23x21" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(JumpMostKings, F)
{
        // Art. 6.8 (jump most kings)
        auto const FEN = "W:WK23:B19,K28";
        auto const legal = std::vector<std::string>{ "23x32" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(JumpFirstKing, F)
{
        // Art. 6.9 (jump first king)
        auto const FEN = "W:WK30:B10,12,18,K20,K26,27";
        auto const legal = std::vector<std::string>{ "30x05" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(EquivalentJump, F)
{
        // Art. 6.10 (equivalent jump)
        auto const FEN = "W:WK31:B18,20,K27,K28";
        auto const legal = std::vector<std::string>{ "31x13", "31x15" };
        run(FEN, begin(legal), end(legal));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
