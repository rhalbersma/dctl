#include <boost/test/unit_test.hpp>
#include <search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestInternational)

using Rules = checkers::Rules;
using Board = board::Checkers;
using F = Fixture< Position<Rules, Board>, DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(Initial, F)
{
        //auto const p = Position<Rules, Board>::initial();
        auto const p = setup::read<Rules, Board, pdn::protocol>()(
                "W:B3,K32:WK18,K19"
        );
        //std::cout << setnotation(numeric) << '\n';
        root_.analyze(p, 71);
}
/*
BOOST_FIXTURE_TEST_CASE(lineoD1, Fixture<DefaultObjective>)
{
        auto const p = setup::read<rules::Lineo, board::International, pdn::protocol>()("W:WK24,K34,K35:BK10");
        root_.analyze(p, 15);
}

BOOST_FIXTURE_TEST_CASE(lineoD2, Fixture<DefaultObjective>)
{
        auto const p = setup::read<rules::Lineo, board::International, pdn::protocol>()("W:WK24,K34,K35:BK9");
        root_.analyze(p, 15);
}
*/

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl
