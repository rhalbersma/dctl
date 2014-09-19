#pragma once

namespace dctl {
namespace precedence {

struct equal_to
{
        template<class Move>
        constexpr auto operator()(Move const&, Move const&) noexcept
        {
                return true;
        }
};

struct less
{
        template<class Move>
        constexpr auto operator()(Move const&, Move const&) noexcept
        {
                return false;
        }
};

}       // namespace precedence
}       // namespace dctl
