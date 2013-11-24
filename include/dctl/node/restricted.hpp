#pragma once
#include <cassert>                      // assert
#include <cstdint>
#include <dctl/bit/bit_set.hpp>
#include <dctl/bit/predicates.hpp>
#include <dctl/utility/ply.hpp>         // PlyCount

namespace dctl {

struct KingMoves
{
public:
        using BitSet = bit::bit_set<int, uint64_t, 1>;

        // structors

        KingMoves() = default;

        // modifiers

        void init(BitSet dest)
        {
                assert(bit::is_single(dest.as_block()));
                king_ = dest;
                moves_ = 1;
                assert(invariant());
        }

        void increment(BitSet dest)
        {
                assert(bit::is_single(dest.as_block()));
                king_ = dest;
                ++moves_;
                assert(invariant());
        }

        void reset()
        {
                assert(!king_.empty() && moves_);
                king_.reset();
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
                        !bit::is_multiple(king_.as_block()) &&
                        king_.empty() == !moves_
                );
        }

        // representation

        BitSet king_{};
        PlyCount moves_{};
};

using Restricted = KingMoves[2];

// predicates
template<class Rules>
auto is_max(PlyCount moves)
{
        return (moves == Rules::max_same_king_moves::value);
}

}       // namespace dctl
