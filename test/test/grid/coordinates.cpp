#include <algorithm>                            // any_of
#include <iterator>                             // begin, end
#include <type_traits>                          // common_type
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // _deg2
#include <dctl/board/types.hpp>                 // Micro, Mini, Checkers, International, Roman, Frisian, Spantsireti, Ktar11, Ktar12
#include <dctl/grid/coordinates.hpp>            // Coordinates, Square, rotate2, sqtocoord
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // make_cyclic

namespace dctl {
namespace board {

BOOST_AUTO_TEST_SUITE(TestCoordinates)

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
        0_deg2
});

auto const C2 = group::make_cyclic({
        0_deg2, 180_deg2
});

auto const C4 = group::make_cyclic({
        0_deg2,  90_deg2, 180_deg2, 270_deg2
});

using CyclicGroup = std::common_type<decltype(C1), decltype(C2), decltype(C4)>::type;

CyclicGroup const C_N[] =
{
        C1, C2, C4
};

BOOST_AUTO_TEST_CASE_TEMPLATE(AllGroupActionIsRealizedOnAllSquareCoordinates, T, BoardSequence)
{
        auto const first = boost::counting_iterator<int>{T::begin()};
        auto const last  = boost::counting_iterator<int>{T::end()  };

        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [=](CyclicGroup const& g) {
                        return std::all_of(first, last, [&](int const& i) {
                                auto const coord = grid::sqtocoord(grid::Square<typename T::ExternalGrid>{i});
                                return group::action::is_realized(coord, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace board
}       // namespace dctl
