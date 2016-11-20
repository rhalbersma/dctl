#include <dctl/rules/italian.hpp>       // italian
#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/color_piece.hpp>         // king, pawn
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, is_superior_rank_jump, is_ordering, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(Rulesitalian)

using T = italian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(!is_long_ranged_king_v<T>);

        static_assert(is_superior_rank_jump_v<T>);

        static_assert(is_nontrivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);
        static_assert(is_quality_precedence_v<T>);
        static_assert(is_ordering_precedence_v<T>);

        struct Action
        {
                using rules_type = italian;
                int num_captured_;
                int num_captured_kings_;
                piece with_;
                std::vector<int> piece_order_;

                auto num_captured_pieces() const noexcept { return num_captured_;        }
                auto num_captured_kings()  const noexcept { return num_captured_kings_;  }
                auto is_with_king()        const noexcept { return with_ == piece::king; }
                auto const& piece_order()  const noexcept { return piece_order_;         }
        };

        auto const moves = std::vector<Action>
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

        BOOST_CHECK(precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
