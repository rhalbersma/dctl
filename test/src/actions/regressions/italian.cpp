#include <actions/fixture.hpp>          // run
#include <dctl/board.hpp>               // roman
#include <dctl/rules.hpp>               // italian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string

namespace dctl {
namespace core {

using F = Fixture<rules::italian, board::roman>;

BOOST_AUTO_TEST_SUITE(ActionsRegressionsitalian)

        BOOST_AUTO_TEST_SUITE(JumpPrecedenceOrderOfKings)

                // Suggested by Alioto Domenico in email to Ed Gilbert.

                BOOST_FIXTURE_TEST_CASE(AliotoDomenico, F)      // Art. 6.9
                {
                        auto const fen = "W:WK30:B10,K12,K18,20,K26,K27";
                        auto const legal = std::vector<std::string>{ "30x05" };
                        test(fen, legal);
                }

        BOOST_AUTO_TEST_SUITE_END()

        BOOST_AUTO_TEST_SUITE(PawnsCannotJumpKings)

                BOOST_FIXTURE_TEST_CASE(NoActions, F)           // Art 5.3(b)
                {
                        auto const fen = "W:W32:BK28";
                        auto const legal = std::vector<std::string>{ /* NO MOVES */ };
                        test(fen, legal);
                }

                BOOST_FIXTURE_TEST_CASE(pawn_push, F)            // Art 5.3(b)
                {
                        auto const fen = "W:W30:BK26";
                        auto const legal = std::vector<std::string>{ "30-27" };
                        test(fen, legal);
                }

        BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}       // namespace core
}       // namespace dctl
