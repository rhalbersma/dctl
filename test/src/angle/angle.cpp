#include <group.hpp>                            // action::is_realized, make
#include <dctl/angle.hpp>                       // _deg, inverse, rotate
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <algorithm>                            // all_of
#include <iterator>                             // begin, end
#include <type_traits>                          // common_type

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleAngle)

BOOST_AUTO_TEST_CASE(GroupActionIsRealizedForRegularCyclicGroupsOnAllAngles)
{
        auto const op = [](auto i, auto j){ return rotate(i, j); };
        auto const inv = [](auto i){ return inverse(i); };

        auto const C1 = make_group(
                { 0_deg },
                op, inv
        );

        auto const C2 = make_group(
                { 0_deg, 180_deg },
                op, inv
        );

        auto const C4 = make_group(
                { 0_deg,  90_deg, 180_deg, 270_deg },
                op, inv
        );

        auto const C8 = make_group(
                {   0_deg,  45_deg,  90_deg, 135_deg,
                  180_deg, 225_deg, 270_deg, 315_deg },
                op, inv
        );

        using CyclicGroup = std::common_type_t<decltype(C1), decltype(C2), decltype(C4), decltype(C8)>;

        CyclicGroup const C_N[] =
        {
                C1, C2, C4, C8
        };

        auto const alpha = boost::counting_iterator<int>{  0};
        auto const omega = boost::counting_iterator<int>{360};

        BOOST_CHECK(
                std::all_of(begin(C_N), end(C_N), [=](auto const& g){
                        return std::all_of(alpha, omega, [&](auto i){
                                return group::action::is_realized(Angle{i}, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
