#include <boost/test/unit_test.hpp>
#include <search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(SearchEGDBFrisian)

using F = Fixture< State<rules::Frisian, board::International>, DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(Walinga, F)
{
        Test const tests[] =
        {
                { "W:WK46,28:BK43", 41 },       // Walinga book
        };
        run(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl
