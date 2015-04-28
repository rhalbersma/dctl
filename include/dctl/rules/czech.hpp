#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // long_ranged_tag

namespace dctl {
namespace czech {

// http://www.damweb.cz/pravidla/cdfull.html

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        // move
        static constexpr auto jumpsep = ':';
        static constexpr auto notation = Notation::algebraic;

        // precedence
        static constexpr auto is_absolute_with_king = true;

        struct equal_to
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) const noexcept
                {
                        return lhs.is_with(Piece::king) == rhs.is_with(Piece::king);
                }
        };

        struct less
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) const noexcept
                {
                        return lhs.is_with(Piece::king) < rhs.is_with(Piece::king);
                }
        };
};

}       // namespace czech
}       // namespace dctl
