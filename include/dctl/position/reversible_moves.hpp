#pragma once
#include <cstddef>      // size_t

namespace dctl {

class ReversibleMoves
{
        std::size_t ply = 0;
public:
        ReversibleMoves() = default;

        explicit ReversibleMoves(std::size_t n) noexcept
        :
                ply{n}
        {}

        /* implicit */ operator auto() const noexcept
        {
                return ply;
        }

        template<class Move>
        void make(Move const& m) noexcept
        {
                if (m.is_reversible())
                        ++ply;
                else
                        ply = 0;
        }
};

}       // namespace dctl
