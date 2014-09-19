#pragma once

namespace dctl {
namespace international {
namespace precedence {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return lhs.num_pieces() == rhs.num_pieces();
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return lhs.num_pieces() < rhs.num_pieces();
        }
};

}       // namespace precedence
}       // namespace international
}       // namespace dctl
