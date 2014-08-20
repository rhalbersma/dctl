#pragma once
#include <cassert>                      // assert
#include <limits>                       // max

namespace dctl {

class ReversibleMoves
{
public:
        // constructors

        ReversibleMoves() = default;

        explicit ReversibleMoves(int n) noexcept
        :
                ply_{n}
        {
                assert(invariant());
        }

        // modifiers

        template<class Move>
        void make(Move const& m) noexcept
        {
                if (m.is_reversible()) {
                        assert(!full());
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

        bool invariant() const
        {
                return 0 <= ply_ && ply_ <= std::numeric_limits<int>::max();
        }

        bool empty() const
        {
                return 0 == ply_;
        }

        bool full() const
        {
                return ply_ == std::numeric_limits<int>::max();
        }

        // representation

        int ply_{};
};

}       // namespace dctl
