#pragma once
#include <dctl/piece.hpp>       // king
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace spanish {
namespace precedence {

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

}       // namespace precedence
}       // namespace spanish
}       // namespace dctl
