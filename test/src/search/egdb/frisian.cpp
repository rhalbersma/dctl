#include <search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(SearchEGDBFrisian)

using F = Fixture< State<rules::Frisian, board::International>, DefaultObjective >;

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
}       // namespace dctl
