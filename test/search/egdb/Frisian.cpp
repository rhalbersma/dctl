#include <test/test_config.hpp>

#if SEARCH_EGDB_FRISIAN == 1

#include <boost/test/unit_test.hpp>
#include <test/search/Fixture.hpp>
#include <dctl/board/Types.hpp>
#include <dctl/rules/Types.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestFrisian)

BOOST_FIXTURE_TEST_CASE(Frisian21, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:WK46,28:BK43", 39),        // Walinga book
                FEN_depth("B:WK1,39,50:B35,45", 39),
                FEN_depth("W:WK1,25:BK33", 21),
                FEN_depth("W:W45,49,50:BK6", 49)
        };
        run<rules::Frisian, board::Frisian>(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl

#endif
