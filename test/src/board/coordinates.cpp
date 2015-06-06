#include <group.hpp>                            // action::is_realized, make_group
#include <dctl/angle.hpp>                       // _deg, inverse, rotate
#include <board/sequence.hpp>                   // Micro, Mini, Checkers, Roman, Spantsireti, International, Frisian, Ktar<10, 11>,
                                                // Ktar<10, 12>, Compact1012, Compact120, Board<12, 10>, Canadian, SriLankan, Dumm
#include <dctl/board/coordinates.hpp>           // operator==, rotate, to_sco
#include <range/v3/all.hpp>
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK
#include <type_traits>                          // common_type

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(GridCoordinates)

BOOST_AUTO_TEST_CASE_TEMPLATE(GroupActionIsRealizedForAllCyclicGroupsOnAllSquareCoordinates, T, BoardSequence)
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

        using CyclicGroup = std::common_type_t<decltype(C1), decltype(C2), decltype(C4)>;

        CyclicGroup const C_N[] =
        {
                C1, C2, C4
        };

        BOOST_CHECK(
                ranges::all_of(C_N, [=](auto const& g) {
                        return ranges::all_of(T::squares(), [&](auto i) {
                                auto const coord = to_sco(i, T::outer_grid);
                                return group::action::is_realized(coord, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
