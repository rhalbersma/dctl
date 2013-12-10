#pragma once

namespace dctl {

struct KingMoves
{
public:
        // structors

        KingMoves() = default;

        // modifiers

        void init(int dest)
        {
                king_ = dest;
                moves_ = 1;
        }

        void increment(int dest)
        {
                king_ = dest;
                ++moves_;
        }

        void reset()
        {
                king_ = -1;
                moves_ = 0;
        }

        // queries

        auto king() const
        {
                return king_;
        }

        auto moves() const
        {
                return moves_;
        }

private:
        // representation

        int king_{-1};
        int moves_{};
};

using Restricted = KingMoves[2];

// predicates
template<class Rules>
auto is_max(int moves)
{
        return (moves == Rules::max_same_king_moves::value);
}

}       // namespace dctl
