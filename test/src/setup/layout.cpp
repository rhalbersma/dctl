#include <iostream>
#include <typeinfo>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/types.hpp>                 // International
#include <dctl/setup/diagram.hpp>               // diagram, bits, squares
#include <dctl/board/algebraic.hpp>
#include <iterator>
#include <dctl/position/position.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(SetupLayout)
/*
using BoardSequence = boost::mpl::vector
<
        board::Micro, board::Mini, board::Checkers, board::Roman, board::International, board::Canadian,
        board::Frisian, board::Spantsireti, board::Ktar11, board::Ktar12
>;

using DimSequence = boost::mpl::vector
<
        grid::Dimensions< 2,  2>,
        grid::Dimensions< 4,  4>,
        grid::Dimensions< 6,  6>,
        grid::Dimensions< 8,  8>,
        grid::Dimensions<10, 10>,
        grid::Dimensions<12, 12>,
        grid::Dimensions< 9,  9, false>,
        grid::Dimensions< 9,  9, true >,
        grid::Dimensions<11,  9, false>,
        grid::Dimensions<11,  9, true >,
        grid::Dimensions<10,  9, false>,
        grid::Dimensions<10,  9, true >,
        grid::Dimensions<11, 10, false>,
        grid::Dimensions<11, 10, true >,
        grid::Dimensions<10, 10, false>,
        grid::Dimensions<10, 10, true >,
        grid::Dimensions<10, 12, false>,
        grid::Dimensions<10, 12, true >,
        grid::Dimensions<12, 10, false>,
        grid::Dimensions<12, 10, true >
>;


BOOST_AUTO_TEST_CASE_TEMPLATE(Layout, T, DimSequence)
{
        std::cout << "------------\n";
        std::cout << T::height << "x" << T::width << ", " << T::inverted << "\n\n";

        using G = board::Board< T, false>;
        std::cout << diagram<G, bits>()() << '\n';
        std::cout << "size = " << G::internal_grid::size << ", angle = " << G::orientation << "\n\n";

        using H = board::Board< T, true >;
        std::cout << diagram<H, bits>()() << '\n';
        std::cout << "size = " << H::internal_grid::size << ", angle = " << H::orientation << "\n\n";
}
*/
using BSequence = boost::mpl::vector
<
        board::Checkers,
        board::Board< 8, 10>,
        board::Board<10,  8>,
        board::International,
        board::Board<10, 12>,
        board::Board<12, 10>,
        board::Compact1210,
        board::Canadian,
        board::Checkers,
        board::Ktar< 8, 10>,
        board::Ktar<10,  8>,
        board::International,
        board::Ktar<10, 11>,
        board::Ktar<10, 12>,
        board::Compact1012,
        board::Ktar<12, 10>,
        board::Canadian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Initial, T, BSequence)
{
        auto const p = Position<international::Rules, T>::initial();
        std::cout << "W = " << T::width << ", H = " << T::height << ", bits = " << T::internal_grid::size << "\n\n";
        std::cout << setup::diagram<pdn::protocol>()(p);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
