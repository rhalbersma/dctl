#include <dctl/rules/frisian.hpp>       // Frisian
#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/piece.hpp>               // king, pawn
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, is_orthogonal_jump, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <cstddef>                      // size_t
#include <vector>                       // vector

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesFrisian)

using T = Frisian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(is_orthogonal_jump_v<T>);

        static_assert(is_nontrivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);
        static_assert(is_quality_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);

        struct Action
        {
                using rules_type = Frisian;
                std::size_t num_captured_;
                std::size_t num_captured_kings_;
                Piece with_;

                constexpr auto num_captured_pieces()       const noexcept { return num_captured_;        }
                constexpr auto num_captured_kings() const noexcept { return num_captured_kings_;  }
                constexpr auto is_with_king()       const noexcept { return with_ == Piece::king; }
        };

        auto const moves = std::vector<Action>
        {
                { 1, 0, Piece::pawn },
                { 1, 0, Piece::king },
                { 1, 1, Piece::pawn },
                { 1, 1, Piece::king },
                { 2, 0, Piece::pawn },
                { 2, 0, Piece::king },
                { 2, 1, Piece::pawn },
                { 2, 1, Piece::king },
                { 3, 0, Piece::pawn },
                { 3, 0, Piece::king },
                { 2, 2, Piece::pawn },
                { 2, 2, Piece::king },
                { 3, 1, Piece::pawn },
                { 3, 1, Piece::king },
                { 4, 0, Piece::pawn },
                { 4, 0, Piece::king }
        };

        BOOST_CHECK(precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace Frisian
}       // namespace dctl
