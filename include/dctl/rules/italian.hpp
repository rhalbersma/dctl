#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // short_ranged_tag
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Italian
{
        static constexpr auto is_backward_pawn_jump = false;    // 5.3(a)
        using king_range_category = short_ranged_tag;           // 4.7

        static constexpr auto is_pawn_jump_king = false;        // 5.3(b)
        static constexpr auto max_repetitions = 4;              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;        // 10.4

        static constexpr auto is_quantity = true;               // 6.6
        static constexpr auto is_modality = true;               // 6.7
        static constexpr auto is_quality = true;                // 6.8
        static constexpr auto is_ordering = true;               // 6.9

        struct equal_to
        {
                template<class Action>
                /*constexpr*/ auto operator()(Action const& lhs, Action const& rhs) const noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.is_with(Piece::king), lhs.num_captured(Piece::king), lhs.piece_order()) ==
                                std::forward_as_tuple(rhs.num_captured(), rhs.is_with(Piece::king), rhs.num_captured(Piece::king), rhs.piece_order())
                        ;
                }
        };

        struct less
        {
                template<class Action>
                /*constexpr*/ auto operator()(Action const& lhs, Action const& rhs) const noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.is_with(Piece::king), lhs.num_captured(Piece::king), lhs.piece_order()) <
                                std::forward_as_tuple(rhs.num_captured(), rhs.is_with(Piece::king), rhs.num_captured(Piece::king), rhs.piece_order())
                        ;
                }
        };
};

}       // namespace rules
}       // namespace dctl
