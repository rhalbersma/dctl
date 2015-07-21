#include <walk/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/walk/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>                      // size_t
#include <vector>

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkInternationalFMJD)

// The "Perft" thread on the FMJD forum (results until depth=9)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=7

BOOST_AUTO_TEST_CASE(Random178)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"
        );
        auto const leafs = std::vector<std::size_t>{ 14, 55, 1'168, 5'432, 87'195, 629'010, 9'041'010, 86'724'219, 1'216'917'193 };
        test(p, unique_gen, leafs);
}

// The "Perft" thread on the FMJD forum (results until depth=15)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=7

BOOST_AUTO_TEST_CASE(Woldouby)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"
        );
        auto const leafs = std::vector<std::size_t>{ 6, 12, 30, 73, 215, 590, 1'944, 6'269, 22'369, 88'050, 377'436, 1'910'989, 9'872'645, 58'360'286, 346'184'885 };
        test(p, unique_gen, leafs);
}
/*
// The "Perft" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=60

BOOST_AUTO_TEST_CASE(AllKings)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                "W:WK31,K32,K33,K34,K35,K36,K37,K38,K39,K40,K41,K42,K43,K44,K45,K46,K47,K48,K49,K50:BK1,K2,K3,K4,K5,K6,K7,K8,K9,K10,K11,K12,K13,K14,K15,K16,K17,K18,K19,K20"
        );
        auto const leafs = std::vector<std::size_t>{ 17, 79, 352, 1'399, 7'062, 37'589, 217'575, 1'333'217, 8'558'321, 58'381'162, 417'920'283 };
        test(p, unique_gen, leafs);
}

// The "Perft" thread on the FMJD forum (results until depth=9)
// http://laatste.info/bb3/viewtopic.php?f=53&t=2308&start=60

BOOST_AUTO_TEST_CASE(AlmostPromotion)
{
        auto const p = setup::read<rules::International, board::International, pdn::protocol>()(
                 "W:W6,7,8,9,10:B41,42,43,44,45"
        );
        auto const leafs = std::vector<std::size_t>{ 9, 81, 795, 7'578, 86'351, 936'311, 11'448'262, 138'362'698, 1'799'526'674 };
        test(p, unique_gen, leafs);
}
*/
BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
