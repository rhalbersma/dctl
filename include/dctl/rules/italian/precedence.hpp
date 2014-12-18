#pragma once
#include <dctl/piece.hpp>       // king
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace italian {
namespace precedence {

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

}       // namespace precedence
}       // namespace italian
}       // namespace dctl
