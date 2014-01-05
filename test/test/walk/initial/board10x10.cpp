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

// The original perft thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308

BOOST_AUTO_TEST_CASE(InternationalInitial)
{
        auto const p = Position<rules::International, board::International>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41022423, 258895763, 1665861398 };
        test(p, leafs);
}

// http://laatste.info/bb3/viewtopic.php?f=53&t=2822&start=15

BOOST_AUTO_TEST_CASE(FrisianInitial)
{
        auto const p = Position<rules::Frisian, board::Frisian>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 3874, 21265, 102431, 540126, 2825779, 15605069, 85817725, 491186430 };
        test(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
