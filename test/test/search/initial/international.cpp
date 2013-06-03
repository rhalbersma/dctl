#include <boost/test/unit_test.hpp>
#include <test/search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/setup/string.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestInternational)

typedef Fixture< Position<rules::International, board::International>, DefaultObjective > F;

BOOST_FIXTURE_TEST_CASE(Initial, F)
{
        auto const p = Position<rules::International, board::International>::initial();
        std::cout << sizeof(decltype(p)) << "\n";
        root_.analyze(p, 17);
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
