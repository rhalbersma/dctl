#include <ai/search/fixture.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>
#include <boost/test/unit_test.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::international;
using F = search::Fixture< State<Rules, Board>, search::DefaultObjective >;

BOOST_AUTO_TEST_SUITE(SearchInitial)

BOOST_FIXTURE_TEST_CASE(I, F)
{
        auto const p = State<Rules, Board>::initial();
        root_.analyze(p, search::unique_gen, 71);
}

BOOST_AUTO_TEST_SUITE_END()
