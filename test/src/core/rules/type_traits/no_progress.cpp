//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // max_same_king_move
#include <dctl/core/rules.hpp>                  // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE_TEMPLATE

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(MaxSameKingMove)

using VariantsZero = boost::mpl::vector
<
        checkers, czech, international, italian, pool, russian, spanish, thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsZero, T, VariantsZero)
{
        static_assert(max_same_king_move_v<T> == 0);
}

using VariantsThree = boost::mpl::vector
<
        frisian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsThree, T, VariantsThree)
{
        static_assert(max_same_king_move_v<T> == 3);
}

BOOST_AUTO_TEST_SUITE_END()
