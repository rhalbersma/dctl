#pragma once
#include <dctl/piece.hpp>       // king

namespace dctl {
namespace czech {
namespace precedence {

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

}       // namespace precedence
}       // namespace czech
}       // namespace dctl
