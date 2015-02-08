#include <successor/fixture.hpp>        // run
#include <dctl/board/types.hpp>         // International
#include <dctl/rules.hpp>               // International
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string
#include <vector>                       // vector

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SucessorRulebookInternational)

// Positions from the official International rules (Italian translation):
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_II.pdf

using F = Fixture<international::Rules, board::International>;

BOOST_FIXTURE_TEST_CASE(KingMoveRange, F)
{
        // Art. 3.9
        auto const FEN = "W:WK23";
        auto const legal = std::vector<std::string>{
                "23-18", "23-12", "23-07", "23-01",
                "23-19", "23-14", "23-10", "23-05",
                "23-28", "23-32", "23-37", "23-41", "23-46",
                "23-29", "23-34", "23-40", "23-45"
        };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(PawnJumpDirections, F)
{
        // Art. 4.2
        auto const FEN = "W:W35:B30,K40";
        auto const legal = std::vector<std::string>{ "35x24", "35x44" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(KingJumpRange, F)
{
        // Art. 4.3
        auto const FEN = "W:WK41:B23";
        auto const legal = std::vector<std::string>{ "41x19", "41x14", "41x10", "41x05" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)
{
        // Art. 4.5
        auto const FEN = "W:W47:B13,14,22,24,31,34,K41,44";
        auto const legal = std::vector<std::string>{ "47x49" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)
{
        // Art. 4.6
        auto const FEN = "W:WK1:B7,9,17,19,20,30,31,33,43,44";
        auto const legal = std::vector<std::string>{ "01x15" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(ApresFiniJumpRemoval, F)
{
        // Art. 4.8
        auto const FEN = "B:W27,28,38,39,42:BK25";
        auto const legal = std::vector<std::string>{ "25x33" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(NumberOfPieces, F)
{
        // Art. 4.13
        auto const FEN = "W:WK48:B7,8,31,34,K42,44";
        auto const legal = std::vector<std::string>{ "48x50" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(NotNumberOfKings, F)
{
        // Art. 4.14
        auto const FEN = "W:W26:B12,K21,31,32";
        auto const legal = std::vector<std::string>{ "26x08", "26x28" };
        run(FEN, begin(legal), end(legal));
}

BOOST_FIXTURE_TEST_CASE(ApresFiniPromotion, F)
{
        // Art. 4.15
        auto const FEN = "W:W15:B9,10";
        auto const legal = std::vector<std::string>{ "15x13" };
        run(FEN, begin(legal), end(legal));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
