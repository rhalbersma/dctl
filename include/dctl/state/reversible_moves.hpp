#pragma once
#include <cstddef>      // size_t

namespace dctl {

class ReversibleActions
{
        std::size_t ply = 0;
public:
        explicit constexpr ReversibleActions(std::size_t n) noexcept
        :
                ply{n}
        {}

        /* implicit */ constexpr operator auto() const noexcept
        {
                return ply;
        }

        ReversibleActions() = default;

        template<class Action>
        constexpr auto make(Action const& m) noexcept
        {
                if (m.is_reversible())
                        ++ply;
                else
                        ply = 0;
        }
};

}       // namespace dctl
