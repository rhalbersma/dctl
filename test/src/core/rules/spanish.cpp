//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/rules/precedence.hpp>            // is_consistent
#include <dctl/core/rules/spanish.hpp>          // spanish
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, is_long_ranged_king, is_trivial_precedence, equal_to, less
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <vector>                               // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(Rulesspanish)

using T = spanish;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(!is_trivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);
        static_assert(is_contents_precedence_v<T>);

        struct Action
        {
                using rules_type [[maybe_unused]] = spanish;
                int m_num_captured_pieces;
                int m_num_captured_kings;
                constexpr auto num_captured_pieces() const noexcept { return m_num_captured_pieces; }
                constexpr auto num_captured_kings()  const noexcept { return m_num_captured_kings;  }
        };

        auto const moves = std::vector<Action>
        {
                { 1, 0 },
                { 1, 1 },
                { 2, 0 },
                { 2, 1 },
                { 2, 2 }
        };

        BOOST_CHECK(xxx_precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()
