#include "../../test_config.hpp"

#if WALK_ITALIAN_GILBERT == 1

#include <boost/test/unit_test.hpp>
#include "../Fixture.hpp"
#include "../../../src/walk/Root.hpp"
#include "../../../src/node/Position.hpp"
#include "../../../src/setup/Setup.hpp"
#include "../../../src/board/Types.hpp"
#include "../../../src/rules/Types.hpp"
#include "../../../src/utility/IntegerTypes.hpp"

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(TestItalianGilbert)

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822
/*
BOOST_FIXTURE_TEST_CASE(Gilbert1, Fixture)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3"
        );
        NodeCount const leafs[] = { 5, 13, 42, 107, 360, 1099, 3736, 12495, 43686, 164177, 628686, 2643623, 10833722, 49327264, 212130912, 1021757399 };
        run(p, leafs);
}

BOOST_FIXTURE_TEST_CASE(Gilbert2, Fixture)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "B:W30,21,22,17,20,K6:B25,28,9,5,1,3"
        );
        NodeCount const leafs[] = { 6, 47, 271, 1916, 10810, 73137, 389809, 2469050, 12803372, 77920042, 396940628, 2365222285 };
        run(p, leafs);
}
*/
BOOST_FIXTURE_TEST_CASE(Gilbert3, Fixture)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "W:WK27,K28,17,20,9,K12,8:B21,24,K19,K13,14,K11,4"
        );
        NodeCount const leafs[] = { 13, 112, 828, 6756, 46241, 368908, 2468110, 19914763, 130562037, 1056217651, 6894949061 };
        run(p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl

#endif
