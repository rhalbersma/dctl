//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/rules/precedence.hpp>            // is_strictly_sorted
#include <dctl/core/rules/italian.hpp>          // italian
#include <dctl/core/state/piece.hpp>            // king, pawn
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial_precedence, is_superior_rank_jump, is_ordering, to_precedence
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <ranges>                               // transform
#include <vector>                               // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(RulesItalian)

using T = italian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(!is_long_ranged_king_v<T>);

        static_assert(is_superior_rank_jump_v<T>);

        static_assert(!is_trivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);
        static_assert(is_contents_precedence_v<T>);
        static_assert(is_ordering_precedence_v<T>);

        struct Action
        {
                using rules_type [[maybe_unused]] = T;
                int m_num_captured_pieces;
                int m_num_captured_kings;
                piece m_with;
                std::vector<int> m_piece_order;

                auto num_captured_pieces() const noexcept { return m_num_captured_pieces;  }
                auto num_captured_kings()  const noexcept { return m_num_captured_kings;   }
                auto is_with_king()        const noexcept { return m_with == piece::king; }
                auto const& piece_order()  const noexcept { return m_piece_order;          }
        };

        auto const jumps = std::vector<Action>
        {
                { 1, 0, piece::pawn, {      } },
                { 1, 0, piece::king, {      } },
                { 1, 1, piece::king, { 0    } },
                { 2, 0, piece::pawn, {      } },
                { 2, 0, piece::king, {      } },
                { 2, 1, piece::king, { 0    } },
                { 2, 1, piece::king, { 1    } },
                { 2, 2, piece::king, { 0, 1 } },
                { 3, 0, piece::pawn, {      } },
                { 3, 0, piece::king, {      } },
                { 3, 1, piece::king, { 0    } },
                { 3, 1, piece::king, { 1    } },
                { 3, 1, piece::king, { 2    } },
                { 3, 2, piece::king, { 0, 1 } },
                { 3, 2, piece::king, { 0, 2 } },
                { 3, 2, piece::king, { 1, 2 } }
        };

        BOOST_CHECK(is_strictly_sorted(jumps | std::views::transform(to_precedence)));
}

BOOST_AUTO_TEST_SUITE_END()
