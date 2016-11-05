#pragma once
#include <dctl/color_piece.hpp>

namespace dctl {

class PlayerToMove
{
        color player_to_move = color::black;

        constexpr auto& flip() noexcept
        {
                player_to_move = !player_to_move;
                return *this;
        }
public:
        PlayerToMove() = default;

        explicit constexpr PlayerToMove(color c) noexcept
        :
                player_to_move{c}
        {}

        /* implicit */ constexpr operator auto() const noexcept
        {
                return player_to_move;
        }

        template<class Action>
        constexpr auto& make(Action&& /* a */) noexcept
        {
                return flip();
        }

        template<class Action>
        constexpr auto& undo(Action&& /* a */) noexcept
        {
                return flip();
        }
};

}       // namespace dctl
