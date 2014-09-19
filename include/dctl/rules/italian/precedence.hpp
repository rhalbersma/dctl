#pragma once
#include <tuple>        // forward_as_tuple

namespace dctl {
namespace italian {
namespace precedence {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.num_pieces(), lhs.num_kings(), lhs.is_with_king(), lhs.king_order()) ==
                        std::forward_as_tuple(rhs.num_pieces(), rhs.num_kings(), rhs.is_with_king(), rhs.king_order())
                ;
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.num_pieces(), lhs.num_kings(), lhs.is_with_king(), lhs.king_order()) <
                        std::forward_as_tuple(rhs.num_pieces(), rhs.num_kings(), rhs.is_with_king(), rhs.king_order())
                ;
        }
};

}       // namespace precedence
}       // namespace italian
}       // namespace dctl
