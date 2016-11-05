#include <board/group.hpp>              // action::is_realized, make
#include <dctl/board/angle.hpp>         // angle, _deg, inverse, rotate
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/irange.hpp>       // irange
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <type_traits>                  // common_type
#include <vector>                       // vector

using namespace dctl::board;

BOOST_AUTO_TEST_SUITE(angleangle)

BOOST_AUTO_TEST_CASE(GroupActionIsRealizedForRegularCyclicGroupsOnAllangles)
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

        auto const C_N = std::vector<CyclicGroup>
        {
                C1, C2, C4, C8
        };

        auto const angles = boost::irange(0, 360);

        BOOST_CHECK(
                boost::algorithm::all_of(C_N, [=](auto const& g){
                        return boost::algorithm::all_of(angles, [&](auto const a){
                                return group::action::is_realized(angle{a}, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
