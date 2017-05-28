#include <algo/search/fixture.hpp>
#include <dctl/core/board.hpp>
#include <dctl/core/rules.hpp>
#include <dctl/core/setup/string.hpp>
#include <boost/test/unit_test.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::international;
using F = aima::search::Fixture< state<Rules, Board>, aima::search::DefaultObjective >;

BOOST_AUTO_TEST_SUITE(Searchinitial)

BOOST_FIXTURE_TEST_CASE(I, F)
{
        auto const p = state<Rules, Board>::initial();
        root_.analyze(aima::search::drop_duplicates_gen, p, 71);
}

BOOST_AUTO_TEST_SUITE_END()
