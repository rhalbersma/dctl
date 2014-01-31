#include <boost/test/unit_test.hpp>
#include <test/walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/position/position.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/utility/int.hpp>         // NodeCount

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialBoard10x10)

// The "Perft" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=4

BOOST_AUTO_TEST_CASE(International)
{
        auto const p = Position<rules::International, board::International>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'022'423, 258'895'763, 1'665'861'398 };
        test(p, leafs);
}

// http://laatste.info/bb3/viewtopic.php?f=53&t=2822&start=15

BOOST_AUTO_TEST_CASE(Frisian)
{
        auto const p = Position<rules::Frisian, board::Frisian>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 3'874, 21'265, 102'431, 540'126, 2'825'779, 15'605'069, 85'817'725, 491'186'430 };
        test(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
