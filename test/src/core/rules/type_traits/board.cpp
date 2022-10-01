//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // initial_position_gap
#include <dctl/core/rules.hpp>                  // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE_TEMPLATE

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(InitialStateGap)

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

BOOST_AUTO_TEST_SUITE_END()
