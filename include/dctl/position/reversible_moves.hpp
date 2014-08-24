#pragma once
#include <cassert>                      // assert

namespace dctl {

class ReversibleMoves
{
public:
        // constructors

        ReversibleMoves() = default;

        explicit ReversibleMoves(int n) noexcept
        :
                ply_{n}
        {}

        // modifiers

        template<class Move>
        void make(Move const& m) noexcept
        {
                if (m.is_reversible()) {
                        ++ply_;
                        assert(!empty());
                } else {
                        ply_ = 0;
                        assert(empty());
                }
        }

        // observers

        /* implicit */ operator int() const noexcept
        {
                return ply_;
        }

private:
        // contracts

        bool empty() const
        {
                return 0 == ply_;
        }

        // representation

        int ply_{};
};

}       // namespace dctl
