#include <board/group.hpp>                      // action::is_realized, make
#include <dctl/board/angle.hpp>                 // _deg, inverse, rotate
#include <board/sequence.hpp>                   // micro, mini, checkers, roman, spantsireti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, compact_10_12, compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/board/detail/dimensions.hpp>     // Dimensions
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK
#include <algorithm>                            // all_of
#include <type_traits>                          // common_type
#include <vector>                               // vector

namespace dctl {
namespace board {
namespace detail {

BOOST_AUTO_TEST_SUITE(GridDimensions)

BOOST_AUTO_TEST_CASE_TEMPLATE(GroupActionIsRealizedForAllCyclicGroupsOnAllDimensions, T, BoardSequence)
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

        auto const C_N = std::vector<CyclicGroup>
        {
                C1, C2, C4
        };

        auto const dim = Dimensions{ T::width, T::height, T::is_inverted };

        BOOST_CHECK(
                std::all_of(C_N.cbegin(), C_N.cend(), [&](auto const& g) {
                        return group::action::is_realized(dim, g);
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace detail
}       // namespace board
}       // namespace dctl
