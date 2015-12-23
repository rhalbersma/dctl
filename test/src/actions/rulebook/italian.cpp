#include <actions/fixture.hpp>          // run
#include <dctl/board/types.hpp>         // Roman
#include <dctl/rules/italian.hpp>       // Italian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <string>                       // string
#include <vector>                       // vector

namespace dctl {
namespace core {

// States from the official Italian rules:
// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

BOOST_AUTO_TEST_SUITE(ActionsRulebookItalian)

using F = Fixture<rules::Italian, board::Roman>;

BOOST_FIXTURE_TEST_CASE(PawnJumpDirections, F)          // Art. 5.6
{
        auto const FEN = "W:W30:B27";
        auto const legal = std::vector<std::string>{ "30x23" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(PawnJumpContinuation, F)        // Art. 5.7
{
        auto const FEN = "W:W31:B12,20,28";
        auto const legal = std::vector<std::string>{ "31x08" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(KingJumpDirection, F)           // Art. 5.8
{
        auto const FEN = "W:WK22:B18,19,27";
        auto const legal = std::vector<std::string>{ "22x13", "22x15", "22x31" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(KingJumpContinuation, F)        // Art. 5.9
{
        auto const FEN = "W:WK27:B5,6,13,15,21,23";
        auto const legal = std::vector<std::string>{ "27x25" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(NumberOfPieces, F)              // Art. 6.6
{
        auto const FEN = "W:WK21,31:B10,18,27";
        auto const legal = std::vector<std::string>{ "31x06" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(WithKing, F)                    // Art. 6.7
{
        auto const FEN = "W:WK23,31:B19,26,27";
        auto const legal = std::vector<std::string>{ "23x21" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(NumberOfKings, F)               // Art. 6.8
{
        auto const FEN = "W:WK23:B19,K28";
        auto const legal = std::vector<std::string>{ "23x32" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(OrderOfKings, F)                // Art. 6.9
{
        auto const FEN = "W:WK30:B10,12,18,K20,K26,27";
        auto const legal = std::vector<std::string>{ "30x05" };
        test(FEN, legal);
}

BOOST_FIXTURE_TEST_CASE(Equivalent, F)                  // Art. 6.10
{
        auto const FEN = "W:WK31:B18,20,K27,K28";
        auto const legal = std::vector<std::string>{ "31x13", "31x15" };
        test(FEN, legal);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace core
}       // namespace dctl
