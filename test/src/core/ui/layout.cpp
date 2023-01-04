//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/sequence.hpp>
#include <dctl/core/action.hpp>
#include <dctl/core/board.hpp>          // international
#include <dctl/core/rules.hpp>
#include <dctl/core/state.hpp>
#include <boost/mpl/vector.hpp>         // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE_TEMPLATE
#include <iostream>

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(SetupLayout)

using RSequence = boost::mpl::vector
<
        checkers,
        czech,
        frisian,
        international,
        italian,
        pool,
        russian,
        spanish,
        thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(initial, T, BoardSequence)
{
        auto const s = basic_state<international, T>::initial();
        std::cout << diag << s;
        std::cout << "W = " << T::width << ", H = " << T::height << ", P = " << T::coloring << ", bits = " << T::bits() << "\n\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ActionSize, T, RSequence)
{
        using A = basic_action<T, basic_board<international>>;
        std::cout << "sizeof(Action) = " << sizeof(A) << "\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(StateSize, T, RSequence)
{
        using state = basic_state<T, basic_board<international>>;
        std::cout << "sizeof(state) = " << sizeof(state) << "(" << sizeof(square_t<basic_mask<board_t<state>>>) << ")" << "\n";
}

BOOST_AUTO_TEST_CASE(Grid)
{
        auto const d = detail::dimensions{1,1,0};
        auto const i = detail::InnerGrid{d};
        auto const o = detail::bit_layout{i, 2};
        std::cout << "W = " << i.width() << ", H = " << i.height() << ", P = " << i.coloring() << ", bits = " << i.size() << "\n\n";
        std::cout << "W = " << o.width() << ", H = " << o.height() << ", P = " << o.coloring() << ", bits = " << o.size() << "\n\n";
}

BOOST_AUTO_TEST_SUITE_END()
