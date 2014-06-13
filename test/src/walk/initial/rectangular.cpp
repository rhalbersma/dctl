#include <boost/test/unit_test.hpp>
#include <walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/position/position.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/utility/int.hpp>         // NodeCount

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialRectangular)

BOOST_AUTO_TEST_CASE(Spantsireti)
{
        auto const p = Position<rules::International, board::Spantsireti>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4'265, 27'117, 167'140, 1'029'319, 6'127'190, 36'751'086, 218'398'167, 1'306'818'894 };
        test(p, leafs);
}

BOOST_AUTO_TEST_CASE(Ktar10x11)
{
        auto const p = Position<rules::International, board::Ktar<10,11>>::initial();
        NodeCount const leafs[] = { 9, 81, 810, 7'583, 74'602, 688'835, 6'555'302, 59'796'721, 558'094'366, 5'058'805'542 };
        test(p, leafs);
}

// The "Perft on rectangular boards" thread on the FMJD forum (results until depth=9)
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
// The "Perft on large boards" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=2

BOOST_AUTO_TEST_CASE(Ktar10x12)
{
        NodeCount const leafs[] = { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'291'394, 263'895'730, 1'731'541'289 };

        // board with International initial moves and no orthogonal capture support: fits into 64 bits
        auto const p = Position<rules::International, board::Ktar<10,12>>::initial();
        test(p, leafs);

        // board with International initial moves and orthogonal capture support: does not fit into 64 bits
        auto const q = Position<rules::International, board::Board<10,12>>::initial();
        test(q, leafs);
}

// The "Perft on large boards" thread on the FMJD forum (results until depth=10)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=1

BOOST_AUTO_TEST_CASE(Ktar12x10)
{
        NodeCount const leafs[] = { 11, 121, 1'222, 10'053, 79'049, 584'100, 4'369'366, 31'839'056, 236'364'607, 1'742'748'504 };

        // board with regular coloring and no orthogonal capture support: fits into 64 bits
        auto const p = Position<rules::International, board::Ktar<12,10>>::initial();
        test(p, leafs);

        // board with Canadian initial moves and orthogonal capture support: does not fit into 64 bits
        auto const q = Position<rules::International, board::Board<12,10>>::initial();
        test(q, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
