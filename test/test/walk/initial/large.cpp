#include <boost/test/unit_test.hpp>
#include <test/walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/position/position.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/utility/int.hpp>         // NodeCount

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialLarge)

// The "Perft on large boards" thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=3

BOOST_AUTO_TEST_CASE(Canadian)
{
        auto const p = Position<rules::International, board::Canadian>::initial();
        NodeCount const leafs[] = { 11, 121, 1222, 10053, 79049, 584100, 4369366, 31839056, 237209258, 1761652936 };
        test(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
