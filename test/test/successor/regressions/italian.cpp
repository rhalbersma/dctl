#include <string>                       // string
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_FIXTURE_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <test/successor/fixture.hpp>   // run
#include <dctl/board/types.hpp>         // Roman
#include <dctl/rules/variants.hpp>      // Italian

namespace dctl {
namespace successor {

BOOST_AUTO_TEST_SUITE(SuccessorRegressionsItalian)

// suggested by Alioto Domenico in email to Ed Gilbert

typedef Fixture<rules::Italian, board::Roman> F;

BOOST_FIXTURE_TEST_CASE(AliotoDomenico, F)
{
        // generalizes the "jump first king" rule (Art. 6.9)
        auto const FEN = "W:WK30:B10,K12,K18,20,K26,K27";
        std::string const legal[] = { "30x5" };
        run(FEN, legal);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace successor
}       // namespace dctl
