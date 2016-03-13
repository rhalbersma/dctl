#include <board/group.hpp>              // action::is_realized, make
#include <dctl/board/angle.hpp>         // angle, _deg, inverse, rotate
#include <range/v3/all.hpp>             // all_of, view::iota
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <type_traits>                  // common_type

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

        auto const angles = ranges::view::iota(0, 360);

        BOOST_CHECK(
                ranges::all_of(C_N, [=](auto const& g){
                        return ranges::all_of(angles, [&](auto a){
                                return group::action::is_realized(angle{a}, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
