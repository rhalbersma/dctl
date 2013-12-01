#include <iostream>
#include <typeinfo>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/board/types.hpp>                 // International
#include <dctl/setup/diagram.hpp>               // diagram, bits, squares

namespace dctl {
namespace setup {

BOOST_AUTO_TEST_SUITE(SetupLayout)
/*
using BoardSequence = boost::mpl::vector
<
        board::Micro, board::Mini, board::Checkers, board::Roman, board::International, board::Canadian,
        board::Frisian, board::Spantsireti, board::Ktar11, board::Ktar12
>;
*/
using DimSequence = boost::mpl::vector
</*
        grid::Dimensions< 2,  2>,
        grid::Dimensions< 4,  4>,
        grid::Dimensions< 6,  6>,
        grid::Dimensions< 8,  8>,
        grid::Dimensions<10, 10>,
        grid::Dimensions<12, 12>,*/
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
        grid::Dimensions<12, 10, false>,
        grid::Dimensions<12, 10, true >
>;


BOOST_AUTO_TEST_CASE_TEMPLATE(Layout, T, DimSequence)
{
        std::cout << "------------\n";
        std::cout << T::height << "x" << T::width << ", " << T::parity << "\n\n";

        static constexpr auto Orth = true;
        using G = board::Board< T, Orth>;
        /*using H = board::Board< T, Orth,  90>;
        using I = board::Board< T, Orth, 180>;
        using J = board::Board< T, Orth, 270>;*/

        std::cout << diagram<G, bits>()() << "\n";
        std::cout << "size = " << G::InternalGrid::size << "(" << G::Index::value << ")" << "\n\n";
/*
        std::cout << diagram<H, bits>()() << "\n";
        std::cout << "size = " << H::InternalGrid::size << "\n\n";

        std::cout << diagram<I, bits>()() << "\n";
        std::cout << "size = " << I::InternalGrid::size << "\n\n";

        std::cout << diagram<J, bits>()() << "\n";
        std::cout << "size = " << J::InternalGrid::size << "\n\n";*/
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace setup
}       // namespace dctl
