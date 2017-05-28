#include <algo/traversal/fixture.hpp>
#include <dctl/core/board.hpp>
#include <dctl/core/state.hpp>
#include <dctl/core/rules.hpp>
#include <dctl/algo/traversal/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>                      // int64_t
#include <vector>

namespace dctl::aima::traversal {

BOOST_AUTO_TEST_SUITE(WalkinitialBoard10x10)

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-69.html

// The "Perft" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=4

BOOST_AUTO_TEST_CASE(international)
{
        auto const p = state<rules::international>::initial();
        auto const leafs = std::vector<std::vector<int64_t>> {
                { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'971, 41'022'614, 258'935'682, 1'666'207'133, 10'752'899'471 },
                { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'022'423, 258'895'763, 1'665'861'398                 }
        };
        test(keep_duplicates_gen, p, leafs[0]);
        test(drop_duplicates_gen, p, leafs[1]);
}
/*
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822&start=15

BOOST_AUTO_TEST_CASE(frisian)
{
        auto const p = state<rules::frisian, board::frisian>::initial();
        auto const leafs = std::vector<int64_t>{ 9, 81, 658, 3'874, 21'265, 102'431, 540'126, 2'825'779, 15'605'069, 85'817'725, 491'186'430 };
        test(p, drop_duplicates_gen, leafs);
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl::aima::traversal
