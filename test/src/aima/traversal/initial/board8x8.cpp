#include <aima/traversal/fixture.hpp>
#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/aima/traversal/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>                      // size_t
#include <vector>

namespace dctl::aima::traversal {

BOOST_AUTO_TEST_SUITE(WalkinitialBoard8x8)

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html

BOOST_AUTO_TEST_CASE(brazilianinitial)
{
        auto const p = state<rules::brazilian, board::brazilian>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'473, 37'628, 187'302, 907'836, 4'431'847, 21'566'606, 105'534'946, 512'171'742, 2'483'592'238 },
                { 7, 49, 302, 1'469, 7'473, 37'628, 187'302, 907'830, 4'431'766, 21'560'022, 105'491'257, 511'882'477, 2'481'546'396 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html

BOOST_AUTO_TEST_CASE(poolinitial)
{
        auto const p = state<rules::pool>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'482, 37'986, 190'146, 929'902, 4'570'615, 22'442'551, 110'877'932, 544'300'084, 2'670'481'140 },
                { 7, 49, 302, 1'469, 7'482, 37'986, 190'146, 929'896, 4'570'534, 22'435'955, 110'833'952, 544'005'148, 2'668'385'616 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_AUTO_TEST_CASE(russianinitial)
{
        auto const p = state<rules::russian>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'482, 37'986, 190'146, 929'905, 4'570'667, 22'450'628, 110'961'169, 545'059'387, 2'675'994'747, 13'138'899'366 },
                { 7, 49, 302, 1'469, 7'482, 37'986, 190'146, 929'899, 4'570'586, 22'444'032, 110'917'189, 544'770'444, 2'673'979'569                 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html

BOOST_AUTO_TEST_CASE(checkersinitial)
{
        auto const p = state<rules::checkers>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'680, 18'391'564, 85'242'128, 388'623'673, 1'766'623'630, 7'978'439'499 },
                { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'680, 18'391'564, 85'242'128, 388'617'999, 1'766'564'893                }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(czechinitial)
{
        auto const p = state<rules::czech>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'671, 18'368'918, 84'967'210, 386'267'783, 1'749'766'090 },
                { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'671, 18'368'918, 84'967'210, 386'262'109, 1'749'707'352 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(spanishinitial)
{
        auto const p = state<rules::spanish>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'361, 36'473, 177'532, 828'783, 3'860'866, 17'743'464, 81'383'200, 365'734'003, 1'638'016'958 },
                { 7, 49, 302, 1'469, 7'361, 36'473, 177'532, 828'783, 3'860'866, 17'743'464, 81'383'200, 365'728'331, 1'637'958'247 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(italianinitial)
{
        auto const p = state<rules::italian>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'361, 36'473, 177'532, 828'783, 3'860'875, 17'761'384, 81'647'058, 367'917'147, 1'655'269'811 },
                { 7, 49, 302, 1'469, 7'361, 36'473, 177'532, 828'783, 3'860'875, 17'761'384, 81'647'058, 367'911'475, 1'655'211'086 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(thaiinitial)
{
        auto const p = state<rules::thai>::initial(2);
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'648, 18'363'523, 84'892'793, 385'719'334, 1'745'725'339 },
                { 7, 49, 302, 1'469, 7'361, 36'768, 179'740, 845'931, 3'963'648, 18'363'523, 84'892'793, 385'713'660, 1'745'666'630 }
        };
        test(p, keep_duplicates_gen, leafs[0]);
        test(p, drop_duplicates_gen, leafs[1]);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl::aima::traversal
