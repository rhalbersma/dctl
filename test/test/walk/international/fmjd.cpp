#include <boost/test/unit_test.hpp>
#include <test/walk/fixture.hpp>
#include <dctl/walk/root.hpp>
#include <dctl/position/position.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>
#include <dctl/utility/int.hpp>         // NodeCount

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInternationalFMJD)

// The original perft thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308

BOOST_AUTO_TEST_CASE(Initial)
{
        auto const p = Position<rules::International, board::International>::initial();
        NodeCount const leafs[] = { 9, 81, 658, 4265, 27117, 167140, 1049442, 6483961, 41022423, 258895763, 1665861398 };
        test(p, leafs);
}

BOOST_AUTO_TEST_CASE(Random178)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"
        );
        NodeCount const leafs[] = { 14, 55, 1168, 5432, 87195, 629010, 9041010, 86724219, 1216917193 };
        test(p, leafs);
}

BOOST_AUTO_TEST_CASE(Woldouby)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        );
        NodeCount const leafs[] = { 6, 12, 30, 73, 215, 590, 1944, 6269, 22369, 88050, 377436, 1910989, 9872645, 58360286, 346184885 };
        test(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
