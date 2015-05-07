#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // long_ranged_tag
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Spanish
{
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        struct equal_to
        {
                template<class Move>
                /*constexpr*/ auto operator()(Move const& lhs, Move const& rhs) const noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king)) ==
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king))
                        ;
                }
        };

        struct less
        {
                template<class Move>
                /*constexpr*/ auto operator()(Move const& lhs, Move const& rhs) const noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king)) <
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king))
                        ;
                }
        };
};

}       // namespace rules
}       // namespace dctl
