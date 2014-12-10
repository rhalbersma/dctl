#include <successor/fixture.hpp>        // run
#include <dctl/board/types.hpp>         // Roman
#include <dctl/rules.hpp>               // Italian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorRegressionsItalian)

using F = Fixture<italian::Rules, board::Roman>;

// suggested by Alioto Domenico in email to Ed Gilbert
BOOST_FIXTURE_TEST_CASE(AliotoDomenico, F)
{
        // Art. 6.9
        auto const FEN = "W:WK30:B10,K12,K18,20,K26,K27";
        auto const legal = std::vector<std::string>{ "30x05" };
        run(FEN, begin(legal), end(legal));
}

// invariant() assertion used to fail in Debug Mode until
// detect() started dispatching on is_pawn_jump_king_t
BOOST_FIXTURE_TEST_CASE(PawnsCannotCaptureKings, F)
{
        // Art 5.3(b)
        auto const FEN = "W:W32:BK28";
        auto const legal = std::vector<std::string>{};
        run(FEN, begin(legal), end(legal));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
