//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/sequence.hpp>
#include <dctl/core/action.hpp>
#include <dctl/core/board.hpp>                  // international
#include <dctl/core/rules.hpp>
#include <dctl/core/state.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <iostream>
#include <iterator>
#include <typeinfo>

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
        auto const s = state<international, T>::initial();
        std::cout << diag << s;
        std::cout << "W = " << T::width << ", H = " << T::height << ", P = " << T::is_inverted << ", bits = " << T::bits() << "\n\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ActionSize, T, RSequence)
{
        using A = action<T, board<international>>;
        std::cout << "sizeof(Action) = " << sizeof(A) << "\n";
}

BOOST_AUTO_TEST_CASE_TEMPLATE(StateSize, T, RSequence)
{
        using S = state<T, board<international>>;
        std::cout << "sizeof(State) = " << sizeof(S) << "(" << sizeof(square_t<board_t<S>>) << ")" << "\n";
}

BOOST_AUTO_TEST_CASE(Grid)
{
        auto const d = detail::dimensions{1,1,true};
        auto const i = detail::InnerGrid{d};
        auto const o = detail::bit_layout{i, 2};
        std::cout << "W = " << i.width() << ", H = " << i.height() << ", P = " << i.is_inverted() << ", bits = " << i.size() << "\n\n";
        std::cout << "W = " << o.width() << ", H = " << o.height() << ", P = " << o.is_inverted() << ", bits = " << o.size() << "\n\n";
}

BOOST_AUTO_TEST_SUITE_END()
