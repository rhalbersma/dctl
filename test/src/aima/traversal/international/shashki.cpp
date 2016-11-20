#include <aima/traversal/fixture.hpp>
#include <dctl/board.hpp>
#include <dctl/state.hpp>
#include <dctl/rules/international.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/aima/traversal/root.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>                      // int64_t
#include <vector>

namespace dctl::aima::traversal {

BOOST_AUTO_TEST_SUITE(Walkinternationalshashki)

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-58.html

BOOST_AUTO_TEST_CASE(Frontrows)
{
        auto const p = setup::read<rules::international, board::international, pdn::protocol>()(
                "W:B16,17,18,19,20:W31,32,33,34,35"
        );
        auto const leafs = std::vector<int64_t>
                { 9, 81, 534, 2'538, 9'052, 29'999, 98'971, 310'315, 1'008'317, 3'174'159, 10'369'533, 34'907'917, 123'162'301, 462'333'827, 1'824'763'597 };
        test(p, keep_duplicates_gen, leafs);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl::aima::traversal
