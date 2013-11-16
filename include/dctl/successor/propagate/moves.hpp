#pragma once
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/bit/bitboard.hpp>                // BitBoard

#include <dctl/bit/bit_set.hpp>
#include <dctl/bit/algorithm.hpp>

namespace dctl {
namespace successor {

template<class Position>
struct Propagate<select::moves, Position>
{
public:
        // structors

        using BitSet = bit::bit_set<int, uint64_t, 1>;

        explicit Propagate(Position const& p)
        :
                not_occupied_(not_occupied(p))
        {}

        // queries

        auto path() const
        {
                return not_occupied_;
        }

private:
        // representation

        BitSet not_occupied_;
};

}       // namespace successor
}       // namespace dctl
