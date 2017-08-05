//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>                 // initial_position_gap
#include <dctl/core/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <dctl/core/board.hpp>
#include <dctl/core/board/bearing.hpp>
#include <dctl/core/state/color_piece.hpp>

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(initialStateGap)

using VariantsTwo = boost::mpl::vector
<
        checkers, czech, frisian, international, italian, pool, russian, spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTwo, T, VariantsTwo)
{
        static_assert(initial_position_gap_v<T> == 2);
}

using VariantsFour = boost::mpl::vector
<
        thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsFour, T, VariantsFour)
{
        static_assert(initial_position_gap_v<T> == 4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Jump, T, VariantsTwo)
{
        constexpr auto Or = bearing_v<board<T>, black_, std::false_type>;
        std::cout << "-------------------------\n";
        std::cout << "orientation = " << Or << "\n";
        std::cout << "J:      ";
        using pjd = pawn_jump_directions<T, Or>;

        meta::foldl_comma<pjd>{}([](auto dir) {
                std::cout << std::setw(3) << decltype(dir){} << ", ";
        });
        std::cout << "\n";

        meta::foldl_comma<pjd>{}([&](auto dir) {
                std::cout << "S: " << std::setw(3) << decltype(dir){} << ": ";
                using pst = pawn_scan_turns<T, Or, decltype(dir){}>;
                meta::foldl_comma<pst>{}([&](auto jum) {
                        std::cout << std::setw(3) << decltype(jum){} << ", ";
                });
                std::cout << "\n";
        });
}

BOOST_AUTO_TEST_SUITE_END()
