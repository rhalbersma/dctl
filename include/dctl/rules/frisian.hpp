#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // long_ranged_tag
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        static constexpr auto is_backward_pawn_jump = true;
        using king_range_category = long_ranged_tag;

        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 3;

        static constexpr auto is_quantity = true;
        static constexpr auto is_quality = true;
        static constexpr auto is_modality = true;

        struct equal_to
        {
                template<class Action1, class Action2>
                constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king), lhs.is_with(Piece::king)) ==
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king), rhs.is_with(Piece::king))
                        ;
                }
        };

        struct less
        {
                template<class Action1, class Action2>
                constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
                {
                        auto const delta_pawns = static_cast<int>(lhs.num_captured(Piece::pawn)) - static_cast<int>(rhs.num_captured(Piece::pawn));
                        auto const delta_kings = static_cast<int>(lhs.num_captured(Piece::king)) - static_cast<int>(rhs.num_captured(Piece::king));

                        // Art. 11
                        if (delta_kings * delta_pawns < 0)
                                // delta_kings and delta_pawns are both non-zero and have opposite sign
                                // [2 n - 1] pawns < [n] kings < [2 n] pawns
                                return delta_pawns + 2 * delta_kings - (delta_kings > 0) <  0;
                        else
                                // delta_kings or delta_pawns is zero or they have equal sign
                                return
                                        std::forward_as_tuple(lhs.num_captured(), lhs.is_with(Piece::king)) <
                                        std::forward_as_tuple(rhs.num_captured(), rhs.is_with(Piece::king))
                                ;
                }
        };
};

}       // namespace rules
}       // namespace dctl
