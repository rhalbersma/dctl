#include <aima/traversal/fixture.hpp>
#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/aima/traversal/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>                      // size_t
#include <vector>

namespace dctl::aima::traversal {

BOOST_AUTO_TEST_SUITE(WalkInitialBoard10x10)

// The "Standard validation generator moves" thread on the Shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-69.html

// The "Perft" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=4

BOOST_AUTO_TEST_CASE(international)
{
        auto const p = State<rules::international, board::international>::initial();
        auto const leafs = std::vector<std::vector<std::size_t>> {
                { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'971, 41'022'614, 258'935'682, 1'666'207'133, 10'752'899'471 },
                { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'022'423, 258'895'763, 1'665'861'398                 }
        };
        test(p, duplicate_gen, leafs[0]);
        test(p,    unique_gen, leafs[1]);
}
/*
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822&start=15

BOOST_AUTO_TEST_CASE(frisian)
{
        auto const p = State<rules::frisian, board::frisian>::initial();
        auto const leafs = std::vector<std::size_t>{ 9, 81, 658, 3'874, 21'265, 102'431, 540'126, 2'825'779, 15'605'069, 85'817'725, 491'186'430 };
        test(p, unique_gen, leafs);
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl::aima::traversal
