#include <ai/walk/fixture.hpp>
#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/ai/walk/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>                      // size_t
#include <vector>

namespace dctl {
namespace walk {

BOOST_AUTO_TEST_SUITE(WalkItalianGilbert)

// The alternative game rules thread on the FMJD forum
// http://laatste.info/bb3/viewtopic.php?f=53&t=2822

BOOST_AUTO_TEST_CASE(Gilbert1)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3"
        );
        auto const leafs = std::vector<std::size_t>{ 5, 13, 42, 107, 360, 1'099, 3'736, 12'495, 43'686, 164'177, 628'686, 2'643'623, 10'833'722, 49'327'264, 212'130'912, 1'021'757'399 };
        test(p, duplicate_gen, leafs);
}

BOOST_AUTO_TEST_CASE(Gilbert2)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "B:W30,21,22,17,20,K6:B25,28,9,5,1,3"
        );
        auto const leafs = std::vector<std::size_t>{ 6, 47, 271, 1'916, 10'810, 73'137, 389'809, 2'469'050, 12'803'372, 77'920'042, 396'940'628, 2'365'222'285 };
        test(p, duplicate_gen, leafs);
}

BOOST_AUTO_TEST_CASE(Gilbert3)
{
        auto const p = setup::read<rules::Italian, board::Roman, pdn::protocol>()(
                "W:WK27,K28,17,20,9,K12,8:B21,24,K19,K13,14,K11,4"
        );
        auto const leafs = std::vector<std::size_t>{ 13, 112, 828, 6'756, 46'241, 368'908, 2'468'110, 19'914'763, 130'562'037, 1'056'217'651, 6'894'949'061 };
        test(p, duplicate_gen, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace walk
}       // namespace dctl
