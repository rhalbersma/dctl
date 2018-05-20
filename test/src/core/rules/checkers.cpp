//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/checkers.hpp>         // checkers
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, is_long_ranged_king, is_trivial_precedence
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

#include <type_traits>

using namespace dctl::core;
using T = checkers;

// primary template handles types that have no nested ::type member:
template< class, class = std::void_t<> >
struct has_type_member : std::false_type { };
 
// specialization recognizes types that do have a nested ::type member:
template< class T >
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type { };

BOOST_AUTO_TEST_SUITE(RulesCheckers)

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(!is_long_ranged_king_v<T>);
        //static_assert(is_trivial_precedence_v<T>);
        //static_assert(!is_ordering_precedence_v<T>);
        static_assert(!has_type_member<T>{});
        //static_assert(bla_v<T> == 42);
}

BOOST_AUTO_TEST_SUITE_END()
