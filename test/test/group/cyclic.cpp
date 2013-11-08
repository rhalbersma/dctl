#include <algorithm>                            // all_of
#include <iterator>                             // begin, end
#include <type_traits>                          // common_type
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle.hpp>                       // _deg, inverse
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/axioms.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // make_cyclic

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(GroupCyclic)

auto const C1 = group::make_cyclic({
          0_deg
});

auto const C2 = group::make_cyclic({
          0_deg, 180_deg
});

auto const C4 = group::make_cyclic({
          0_deg,  90_deg, 180_deg, 270_deg
});

auto const C8 = group::make_cyclic({
          0_deg,  45_deg,  90_deg, 135_deg,
        180_deg, 225_deg, 270_deg, 315_deg
});

using CyclicGroup = std::common_type<decltype(C1), decltype(C2), decltype(C4), decltype(C8)>::type;

CyclicGroup const C_N[] =
{
        C1, C2, C4, C8
};

BOOST_AUTO_TEST_CASE(GroupAxiomsAreRealizedOnAllCyclicGroups)
{
        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [](auto const& g){
                        return axioms::is_realized(g);
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
