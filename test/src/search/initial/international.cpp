#include <boost/test/unit_test.hpp>
#include <search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestInternational)

using Rules = russian::Rules;
using Board = board::Checkers;
using F = Fixture< Position<Rules, Board>, DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(Initial, F)
{
        auto const p = Position<Rules, Board>::initial();
        std::cout << sizeof(decltype(p)) << " " << sizeof(decltype(p)::Set) << '\n';
        root_.analyze(p, 37);
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
