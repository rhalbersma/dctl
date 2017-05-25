#include <dctl/rules/frisian.hpp>       // frisian
#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/board.hpp>
#include <dctl/color_piece.hpp>         // king, pawn
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial_precedence, is_orthogonal_jump, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

using namespace dctl;

BOOST_AUTO_TEST_SUITE(Rulesfrisian)

using T = rules::frisian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(is_orthogonal_jump_v<T>);

        static_assert(!is_trivial_precedence_v<T>);
        static_assert(is_quantity_precedence_v<T>);
        static_assert(is_contents_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);

        struct action
        {
                using rules_type = rules::frisian;
		using board_type = board::frisian;
                int num_captured_;
                int num_captured_kings_;
                piece with_;

                constexpr auto num_captured_pieces() const noexcept { return num_captured_;        }
                constexpr auto num_captured_kings()  const noexcept { return num_captured_kings_;  }
                constexpr auto is_with_king()        const noexcept { return with_ == piece::kings; }
        };

        auto const moves = std::vector<action>
        {
                { 1, 0, piece::pawns },
                { 1, 0, piece::kings },
                { 1, 1, piece::pawns },
                { 1, 1, piece::kings },
                { 2, 0, piece::pawns },
                { 2, 0, piece::kings },
                { 2, 1, piece::pawns },
                { 2, 1, piece::kings },
                { 3, 0, piece::pawns },
                { 3, 0, piece::kings },
                { 2, 2, piece::pawns },
                { 2, 2, piece::kings },
                { 3, 1, piece::pawns },
                { 3, 1, piece::kings },
                { 4, 0, piece::pawns },
                { 4, 0, piece::kings }
        };

        BOOST_CHECK(precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()
