#include <test/config.hpp>

#if SEARCH_EGDB_FRISIAN == 1

#include <boost/test/unit_test.hpp>
#include <test/search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestFrisian)

typedef Fixture< Position<rules::Frisian, board::International>, DefaultObjective > FrisianFixture;

BOOST_FIXTURE_TEST_CASE(Frisian21, FrisianFixture)
{
        FEN_depth tests[] = {
                FEN_depth("W:WK46,28:BK43", 39),        // Walinga book
        };
        run(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl

#endif
