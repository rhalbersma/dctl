#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // short_ranged_tag
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace italian {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;    // 5.3(a)
        using king_range_category = short_ranged_tag;           // 4.7

        // optional
        static constexpr auto is_pawn_jump_king = false;        // 5.3(b)

        // draw
        static constexpr auto max_repetitions = 4;              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;        // 10.4

        // precedence
        static constexpr auto is_king_order = true;             // 6.9

        struct equal_to
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king), lhs.is_with(Piece::king), lhs.king_order()) ==
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king), rhs.is_with(Piece::king), rhs.king_order())
                        ;
                }
        };

        struct less
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king), lhs.is_with(Piece::king), lhs.king_order()) <
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king), rhs.is_with(Piece::king), rhs.king_order())
                        ;
                }
        };
};

}       // namespace italian
}       // namespace dctl
