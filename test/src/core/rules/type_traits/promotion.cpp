//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // is_passing_promotion
#include <dctl/core/rules.hpp>                  // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE_TEMPLATE

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(PromotionCategory)

using VariantsStoppedpromotionTag = boost::mpl::vector
<
        checkers, czech, frisian, international, italian, pool, spanish, thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsStoppedpromotionTag, T, VariantsStoppedpromotionTag)
{
        static_assert(!is_passing_promotion_v<T>);
}

using VariantsPassingpromotionTag = boost::mpl::vector
<
        russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPassingpromotionTag, T, VariantsPassingpromotionTag)
{
        static_assert(is_passing_promotion_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()
