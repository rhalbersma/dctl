#include <boost/test/unit_test.hpp>
#include <test/walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/node/position.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInitialRectangular)

BOOST_AUTO_TEST_CASE(Spantsireti)
{
        auto const p = Position<rules::International, board::Spantsireti>::initial();
        NodeCount const leafs[] = { 7, 49, 302, 1469, 7473, 37628, 193605, 995303, 5222464, 27405228, 146853805, 792676205, 4359752501 };
        test(p, leafs);
}

BOOST_AUTO_TEST_CASE(Ktar11)
{
        auto const p = Position<rules::International, board::Ktar11>::initial();
        NodeCount const leafs[] = { 9, 81, 810, 7583, 74602, 688835, 6555302, 59796721, 558094366, 5058805542, 46687246842 };
        test(p, leafs);
}

// The rectangular boards thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014

BOOST_AUTO_TEST_CASE(Ktar12)
{
        auto const p = Position<rules::International, board::Ktar12>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41291394, 263895730, 1731541289 };
        test(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
