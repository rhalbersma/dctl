#include <dctl/board.hpp>
#include <dctl/rules/frisian.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include "../../../../include/aima/search/fixture.hpp"

namespace dctl {
namespace aima {
namespace search {

BOOST_AUTO_TEST_SUITE(SearchEGDBfrisian)

using F = Fixture< State<rules::frisian, board::international>, DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(Walinga, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:WK46,28:BK43", 41 },       // Walinga book
        };
        run(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace aima
}       // namespace dctl
