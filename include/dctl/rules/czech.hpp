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

        // precedence
        struct equal_to
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
                {
                        return lhs.is_with(Piece::king) == rhs.is_with(Piece::king);
                }
        };

        struct less
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
                {
                        return lhs.is_with(Piece::king) < rhs.is_with(Piece::king);
                }
        };

        static constexpr auto is_absolute_with_king = true;

        // move
        static constexpr auto jumpsep = ':';
};

}       // namespace czech
}       // namespace dctl
