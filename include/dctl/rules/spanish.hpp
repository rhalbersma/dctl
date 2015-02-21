#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // long_ranged_tag
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace spanish {

// http://fpdamas.home.sapo.pt/regrasclass.htm

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
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king)) ==
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king))
                        ;
                }
        };

        struct less
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
                {
                        return
                                std::forward_as_tuple(lhs.num_captured(), lhs.num_captured(Piece::king)) <
                                std::forward_as_tuple(rhs.num_captured(), rhs.num_captured(Piece::king))
                        ;
                }
        };
};

}       // namespace spanish
}       // namespace dctl
