#pragma once
#include <cstddef>      // size_t

namespace dctl {

class ReversibleMoves
{
        std::size_t ply = 0;
public:
        explicit constexpr ReversibleMoves(std::size_t n) noexcept
        :
                ply{n}
        {}

        /* implicit */ constexpr operator auto() const noexcept
        {
                return ply;
        }

        ReversibleMoves() = default;

        template<class Move>
        constexpr auto make(Move const& m) noexcept
        {
                if (m.is_reversible())
                        ++ply;
                else
                        ply = 0;
        }
};

}       // namespace dctl
