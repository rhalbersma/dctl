#pragma once
#include <cstddef>      // size_t

namespace dctl {

class ReversibleActions
{
        std::size_t reversible_actions = 0;
public:
        ReversibleActions() = default;

        explicit constexpr ReversibleActions(std::size_t n) noexcept
        :
                reversible_actions{n}
        {}

        /* implicit */ constexpr operator auto() const noexcept
        {
                return reversible_actions;
        }

        template<class Action>
        constexpr auto make(Action const& a) noexcept
        {
                if (a.is_reversible()) {
                        ++reversible_actions;
                } else {
                        reversible_actions = 0;
                }
        }

        template<class Action>
        constexpr auto undo(Action const& a) noexcept
        {
                reversible_actions = a.reversible_actions();
        }
};

}       // namespace dctl
