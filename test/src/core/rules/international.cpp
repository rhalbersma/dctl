//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/rules/precedence.hpp>            // is_strictly_sorted
#include <dctl/core/rules/international.hpp>    // international
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial_precedence, capture_precedence
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <ranges>                               // views::transform
#include <vector>                               // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(RulesInternational)

using T = international;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(!is_trivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);

        struct Action
        {
                using rules_type [[maybe_unused]] = T;
                int m_num_captured_pieces;
                constexpr auto num_captured_pieces() const noexcept { return m_num_captured_pieces; }
        };

        auto const captures = std::vector<Action>
        {
                { 1 },
                { 2 },
                { 3 }
        } | std::views::transform([](auto const& action) { return capture_precedence(action); });

        BOOST_CHECK(is_strictly_sorted(captures));
}

BOOST_AUTO_TEST_SUITE_END()
