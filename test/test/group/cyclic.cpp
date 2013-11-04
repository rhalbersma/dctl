#include <algorithm>                            // all_of
#include <iterator>                             // begin, end
#include <type_traits>                          // common_type
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // _deg2
#include <dctl/angle/transform.hpp>             // inverse2
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/axioms.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // make_cyclic

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(GroupCyclic)

auto const C1 = group::make_cyclic({
          0_deg2
});

auto const C2 = group::make_cyclic({
          0_deg2, 180_deg2
});

auto const C4 = group::make_cyclic({
          0_deg2,  90_deg2, 180_deg2, 270_deg2
});

auto const C8 = group::make_cyclic({
          0_deg2,  45_deg2,  90_deg2, 135_deg2,
        180_deg2, 225_deg2, 270_deg2, 315_deg2
});

using CyclicGroup = std::common_type<decltype(C1), decltype(C2), decltype(C4), decltype(C8)>::type;

CyclicGroup const C_N[] =
{
        C1, C2, C4, C8
};

BOOST_AUTO_TEST_CASE(AllGroupAxiomsIsRealized)
{
        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [](auto const& g){
                        return axioms::is_realized(g);
                })
        );
}

BOOST_AUTO_TEST_CASE(AllGroupActionIsRealizedOnAllAngles)
{
        auto const alpha = boost::counting_iterator<int>{  0};
        auto const omega = boost::counting_iterator<int>{360};

        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [=](CyclicGroup const& g){
                        return std::all_of(alpha, omega, [&](int const& i){
                                return action::is_realized(Angle{i}, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
