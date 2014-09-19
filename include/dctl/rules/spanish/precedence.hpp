#pragma once
#include <tuple>        // forward_as_tuple

namespace dctl {
namespace spanish {
namespace precedence {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.num_pieces(), lhs.num_kings()) ==
                        std::forward_as_tuple(rhs.num_pieces(), rhs.num_kings())
                ;
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.num_pieces(), lhs.num_kings()) <
                        std::forward_as_tuple(rhs.num_pieces(), rhs.num_kings())
                ;
        }
};

}       // namespace precedence
}       // namespace spanish
}       // namespace dctl
