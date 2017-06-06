#include <algo/search/fixture.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

using namespace dctl::algo;
using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(SearchEGDBfrisian)

using F = search::Fixture< state<frisian, board::international>, search::DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(Walinga, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:WK46,28:BK43", 41 },       // Walinga book
        };
        run(tests);
}

BOOST_AUTO_TEST_SUITE_END()
