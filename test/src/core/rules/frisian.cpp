//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/rules/precedence.hpp>            // is_strictly_sorted
#include <dctl/core/rules/frisian.hpp>          // frisian
#include <dctl/core/board.hpp>                  // basic_board
#include <dctl/core/state/piece.hpp>            // king, pawn
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial_precedence, is_orthogonal_jumps, to_precedence
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <ranges>                               // transform
#include <vector>                               // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(RulesFrisian)

using T = frisian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(is_orthogonal_jumps_v<T>);

        static_assert(!is_trivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);
        static_assert(is_contents_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);

        struct Action
        {
                using rules_type [[maybe_unused]] = T;
                using board_type [[maybe_unused]] = basic_board<T>;
                int m_num_captured_pieces;
                int m_num_captured_kings;
                piece m_with;

                constexpr auto num_captured_pieces() const noexcept { return m_num_captured_pieces; }
                constexpr auto num_captured_kings()  const noexcept { return m_num_captured_kings;  }
                constexpr auto is_with_king()        const noexcept { return m_with == piece::king; }
        };

        auto const jumps = std::vector<Action>
        {
                { 1, 0, piece::pawn },
                { 1, 0, piece::king },
                { 1, 1, piece::pawn },
                { 1, 1, piece::king },
                { 2, 0, piece::pawn },
                { 2, 0, piece::king },
                { 2, 1, piece::pawn },
                { 2, 1, piece::king },
                { 3, 0, piece::pawn },
                { 3, 0, piece::king },
                { 2, 2, piece::pawn },
                { 2, 2, piece::king },
                { 3, 1, piece::pawn },
                { 3, 1, piece::king },
                { 4, 0, piece::pawn },
                { 4, 0, piece::king }
        };

        BOOST_CHECK(is_strictly_sorted(jumps | std::views::transform(to_precedence)));
}

BOOST_AUTO_TEST_SUITE_END()
