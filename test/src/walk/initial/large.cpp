#include <boost/test/unit_test.hpp>
#include <walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/position/position.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/utility/int.hpp>         // NodeCount

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialLarge)

// The "Perft on large boards" thread on the FMJD forum (results until depth=10)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=3

BOOST_AUTO_TEST_CASE(Board12x12)
{
        NodeCount const leafs[] = { 11, 121, 1'222, 10'053, 79'049, 584'100, 4'369'366, 31'839'056, 237'209'258, 1'761'652'936 };

        auto const p = Position<rules::International, board::Canadian>::initial();
        test(p, leafs);

        auto const q = Position<rules::International, board::SriLankan>::initial();
        test(q, leafs);
}

BOOST_AUTO_TEST_CASE(Board14x14)
{
        auto const p = Position<rules::International, board::Dumm>::initial();
        NodeCount const leafs[] = { 13, 169, 2'042, 20'513, 195'333, 1'710'812, 15'007'858, 127'249'292, 1'093'968'733 };

        test(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
