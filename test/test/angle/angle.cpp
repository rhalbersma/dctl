#include <algorithm>                            // all_of
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle.hpp>                       // Angle, _deg
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // make_cyclic

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleAngle)

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

BOOST_AUTO_TEST_CASE(GroupActionIsRealizedForAllCyclicGroupsOnAllAngles)
{
        auto const alpha = boost::counting_iterator<int>{  0};
        auto const omega = boost::counting_iterator<int>{360};

        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [=](auto const& g){
                        return std::all_of(alpha, omega, [&](auto i){
                                return group::action::is_realized(Angle{i}, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
