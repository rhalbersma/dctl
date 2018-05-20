//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>                 // is_halt_behind_king
#include <dctl/core/rules.hpp>                       // checkers, czech, frisian, international, italian, killer, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_base_of

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(RulesKiller)

using Variants = boost::mpl::vector
<
        checkers, czech, frisian, international, italian, pool, russian, spanish, thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(RuleTraits, T, Variants)
{
        static_assert(std::is_base_of_v<T, killer<T>>);
        static_assert(is_halt_behind_king_v<killer<T>>);
}

BOOST_AUTO_TEST_SUITE_END()
