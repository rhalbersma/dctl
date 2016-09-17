#include <aima/search/fixture.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>
#include <boost/test/unit_test.hpp>

using namespace dctl;

using Rules = rules::International;
using Board = board::International;
using F = aima::search::Fixture< state<Rules, Board>, aima::search::DefaultObjective >;

BOOST_AUTO_TEST_SUITE(Searchinitial)

BOOST_FIXTURE_TEST_CASE(I, F)
{
        auto const p = state<Rules, Board>::initial();
        root_.analyze(p, aima::search::drop_duplicates_gen, 71);
}

BOOST_AUTO_TEST_SUITE_END()
