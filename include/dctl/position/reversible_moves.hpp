#pragma once
#include <cassert>                      // assert
#include <climits>                      // max

namespace dctl {

class ReversibleMoves
{
public:
        // structors

        ReversibleMoves() = default;

        explicit ReversibleMoves(int n) noexcept
        :
                num_ply_{n}
        {
                assert(invariant());
        }

        // modifiers

        template<class Move>
        void make(Move const& m) noexcept
        {
                if (m.is_reversible()) {
                        assert(!full());
                        ++num_ply_;
                        assert(!empty());
                } else {
                        num_ply_ = 0;
                        assert(empty());
                }
        }

        // queries

        /* implicit */ operator int() const noexcept
        {
                return num_ply_;
        }

private:
        // contracts

        bool invariant() const
        {
                return 0 <= num_ply_ && num_ply_ <= std::numeric_limits<int>::max();
        }

        bool empty() const
        {
                return 0 == num_ply_;
        }

        bool full() const
        {
                return num_ply_ == std::numeric_limits<int>::max();
        }

        // representation

        int num_ply_{};
};

}       // namespace dctl
