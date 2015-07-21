#pragma once
#include <dctl/color.hpp>

namespace dctl {

class PlayerToMove
{
        Color player_to_move = Color::black;

        constexpr auto flip() noexcept
        {
                player_to_move = !player_to_move;
        }
public:
        PlayerToMove() = default;

        explicit constexpr PlayerToMove(Color c) noexcept
        :
                player_to_move{c}
        {}

        /* implicit */ constexpr operator auto() const noexcept
        {
                return player_to_move;
        }

        template<class Action>
        constexpr auto& make(Action const& /* a */) noexcept
        {
                flip();
                return *this;
        }

        template<class Action>
        constexpr auto& undo(Action const& /* a */) noexcept
        {
                flip();
                return *this;
        }
};

}       // namespace dctl
