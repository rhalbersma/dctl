#include <algorithm>                            // any_of
#include <iterator>                             // begin, end
#include <type_traits>                          // common_type
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // _deg
#include <dctl/board/types.hpp>                 // Micro, Mini, Checkers, International, Roman, Frisian, Spantsireti, Ktar11, Ktar12
#include <dctl/grid/dimensions.hpp>             // Dimensions
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // make_cyclic

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestDimensions)

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        International,
        Roman,
        Frisian,
        Spantsireti,
        Ktar11,
        Ktar12
>;

auto const C1 = group::make_cyclic({
        0_deg
});

auto const C2 = group::make_cyclic({
        0_deg, 180_deg
});

auto const C4 = group::make_cyclic({
        0_deg,  90_deg, 180_deg, 270_deg
});

using CyclicGroup = std::common_type<decltype(C1), decltype(C2), decltype(C4)>::type;

CyclicGroup const C_N[] =
{
        C1, C2, C4
};

BOOST_AUTO_TEST_CASE_TEMPLATE(AllGroupActionIsRealizedOnAllDimensions, T, BoardSequence)
{
        auto const dim = grid::DimensionsObject{ T::height, T::width, T::parity };

        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [&](auto const& g) {
                        return group::action::is_realized(dim, g);
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
