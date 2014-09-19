#pragma once

namespace dctl {
namespace czech {
namespace precedence {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return lhs.is_with_king() == rhs.is_with_king();
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const& lhs, Move const& rhs) noexcept
        {
                return lhs.is_with_king() < rhs.is_with_king();
        }
};

}       // namespace precedence
}       // namespace czech
}       // namespace dctl
