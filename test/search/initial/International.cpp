#include <boost/test/unit_test.hpp>
#include "../Fixture.hpp"
#include "../../test_config.hpp"
#include "../../../src/board/Types.hpp"
#include "../../../src/rules/Types.hpp"
#include "../../../src/setup/String.hpp"

#if SEARCH_INITIAL_INTERNATIONAL == 1

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestInternational)

BOOST_FIXTURE_TEST_CASE(InternationalInitial, Fixture<DefaultObjective>)
{
        auto const p = Position<rules::International, board::International>::initial();
        root_.analyze(p, 15);
}
/*
BOOST_FIXTURE_TEST_CASE(LineoD1, Fixture<DefaultObjective>)
{
        auto const p = setup::read<rules::Lineo, board::International, pdn::protocol>()("W:WK24,K34,K35:BK10");
        root_.analyze(p, 15);
}

BOOST_FIXTURE_TEST_CASE(LineoD2, Fixture<DefaultObjective>)
{
        auto const p = setup::read<rules::Lineo, board::International, pdn::protocol>()("W:WK24,K34,K35:BK9");
        root_.analyze(p, 15);
}
*/

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl

#endif
