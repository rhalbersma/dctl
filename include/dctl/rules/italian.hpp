#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // short_ranged_tag
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Italian
{
        using king_range_category = short_ranged_tag;           // 4.7

        static constexpr auto is_backward_pawn_jump = false;    // 5.3(a)
        static constexpr auto is_pawn_jump_king = false;        // 5.3(b)

        static constexpr auto is_quantity = true;               // 6.6
        static constexpr auto is_modality = true;               // 6.7
        static constexpr auto is_quality = true;                // 6.8
        static constexpr auto is_ordering = true;               // 6.9

        struct precedence
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured(), a.is_with(Piece::king), a.num_captured(Piece::king), a.piece_order()
                        );
                }
        };

        static constexpr auto max_repetitions = 4;              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;        // 10.4
};

}       // namespace rules
}       // namespace dctl
