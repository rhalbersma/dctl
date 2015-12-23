#include <actions/fixture.hpp>          // run
#include <dctl/board/types.hpp>         // International
#include <dctl/rules/international.hpp> // International
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string
#include <vector>                       // vector

namespace dctl {
namespace core {

using F = Fixture<rules::International, board::International>;

BOOST_AUTO_TEST_SUITE(ActionsRulebookInternational)

        // States from the International rules (French tutorial):
        // http://www.ffjd.fr/Web/index.php?page=reglesdujeu

        BOOST_AUTO_TEST_SUITE(FrenchTutorial)

                BOOST_FIXTURE_TEST_CASE(WhitePawnPushDirections, F)     // Art. 3.4
                {
                        auto const FEN = "W:W28";
                        auto const legal = std::vector<std::string>{ "28-22", "28-23" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(BlackPawnPushDirections, F)     // Art. 3.4
                {
                        auto const FEN = "B:B13";
                        auto const legal = std::vector<std::string>{ "13-18", "13-19" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(WhiteKingPushRange, F)          // Art. 3.9
                {
                        auto const FEN = "W:WK28";
                        auto const legal = std::vector<std::string>{
                                "28-22", "28-17", "28-11", "28-06",
                                "28-23", "28-19", "28-14", "28-10", "28-05",
                                "28-32", "28-37", "28-41", "28-46",
                                "28-33", "28-39", "28-44", "28-50"
                        };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(BlackKingPushRange, F)          // Art. 3.9
                {
                        auto const FEN = "B:BK1";
                        auto const legal = std::vector<std::string>{
                                "01-06",
                                "01-07", "01-12", "01-18", "01-23",
                                "01-29", "01-34", "01-40", "01-45"
                        };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(PawnJump, F)                    // Art. 4.2
                {
                        auto const FEN = "W:W28,32:B23";
                        auto const legal = std::vector<std::string>{ "28x19" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingJumpRange, F)               // Art. 4.3
                {
                        auto const FEN = "W:WK46:B23";
                        auto const legal = std::vector<std::string>{
                                "46x19", "46x14", "46x10", "46x05"
                        };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)        // Art. 4.5
                {
                        auto const FEN = "W:W15:B8,19,20";
                        auto const legal = std::vector<std::string>{ "15x02" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)        // Art. 4.6
                {
                        auto const FEN = "W:WK47:B9,12,33";
                        auto const legal = std::vector<std::string>{
                                "47x17", "47x21", "47x26"
                        };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(MultipleJumps, F)               // Art. 4.8
                {
                        auto const FEN = "W:WK41,49:B9,10,12,19,38";
                        auto const legal = std::vector<std::string>{ "41x43" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(JumpPrecedence, F)              // Art. 4.13
                {
                        auto const FEN = "W:W48:B24,K31,34,K42,43";
                        auto const legal = std::vector<std::string>{ "48x19" };
                        test(FEN, legal);
                }

        BOOST_AUTO_TEST_SUITE_END()

        // States from the official International rules (Italian translation):
        // http://www.fid.it/regolamenti/2008/RegTec_CAPO_II.pdf

        BOOST_AUTO_TEST_SUITE(ItalianOfficialVersion)

                BOOST_FIXTURE_TEST_CASE(KingPushRange, F)               // Art. 3.9
                {
                        auto const FEN = "W:WK23";
                        auto const legal = std::vector<std::string>{
                                "23-18", "23-12", "23-07", "23-01",
                                "23-19", "23-14", "23-10", "23-05",
                                "23-28", "23-32", "23-37", "23-41", "23-46",
                                "23-29", "23-34", "23-40", "23-45"
                        };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(PawnJumpDirections, F)          // Art. 4.2
                {
                        auto const FEN = "W:W35:B30,K40";
                        auto const legal = std::vector<std::string>{ "35x24", "35x44" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingJumpRange, F)               // Art. 4.3
                {
                        auto const FEN = "W:WK41:B23";
                        auto const legal = std::vector<std::string>{ "41x19", "41x14", "41x10", "41x05" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)        // Art. 4.5
                {
                        auto const FEN = "W:W47:B13,14,22,24,31,34,K41,44";
                        auto const legal = std::vector<std::string>{ "47x49" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)        // Art. 4.6
                {
                        auto const FEN = "W:WK1:B7,9,17,19,20,30,31,33,43,44";
                        auto const legal = std::vector<std::string>{ "01x15" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(ApresFiniJumpRemoval, F)        // Art. 4.8
                {
                        auto const FEN = "B:W27,28,38,39,42:BK25";
                        auto const legal = std::vector<std::string>{ "25x33" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(NumberOfPieces, F)              // Art. 4.13
                {
                        auto const FEN = "W:WK48:B7,8,31,34,K42,44";
                        auto const legal = std::vector<std::string>{ "48x50" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(NotNumberOfKings, F)            // Art. 4.14
                {
                        auto const FEN = "W:W26:B12,K21,31,32";
                        auto const legal = std::vector<std::string>{ "26x08", "26x28" };
                        test(FEN, legal);
                }

                BOOST_FIXTURE_TEST_CASE(ApresFiniPromotion, F)          // Art. 4.15
                {
                        auto const FEN = "W:W15:B9,10";
                        auto const legal = std::vector<std::string>{ "15x13" };
                        test(FEN, legal);
                }

        BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}       // namespace core
}       // namespace dctl
