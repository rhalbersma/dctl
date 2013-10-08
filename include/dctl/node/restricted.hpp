#pragma once
#include <cassert>                      // assert
#include <dctl/bit/bit.hpp>
#include <dctl/bit/bitboard.hpp>        // BitBoard
#include <dctl/utility/ply.hpp>         // PlyCount

namespace dctl {

struct KingMoves
{
public:
        // structors

        KingMoves() = default;

        // modifiers

        void init(BitBoard dest)
        {
                assert(bit::is_single(dest));
                king_ = dest;
                moves_ = 1;
                assert(invariant());
        }

        void increment(BitBoard dest)
        {
                assert(bit::is_single(dest));
                king_= dest;
                ++moves_;
                assert(invariant());
        }

        void reset()
        {
                assert(!bit::empty(king_) && moves_);
                king_ = 0;
                moves_ = 0;
                assert(invariant());
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
        // predicates

        bool invariant() const
        {
                return (
                        !bit::is_multiple(king_) &&
                        bit::empty(king_) == !moves_
                );
        }

        // representation

        BitBoard king_ {};
        PlyCount moves_ {};
};

using Restricted = KingMoves[2];

// predicates
template<class Rules>
auto is_max(PlyCount moves)
{
        return (moves == Rules::max_same_king_moves::value);
}

}       // namespace dctl
