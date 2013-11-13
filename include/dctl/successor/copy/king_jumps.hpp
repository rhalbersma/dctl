#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // copy (king jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/king.hpp>             // king

#include <cstdint>
#include <dctl/bit/bit_set.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps
template<bool Color>
struct copy<Color, pieces::king, select::jumps>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using BitSet = bit::bit_set<int, uint64_t, 1>;
                using KingJumps = impl::copy<Color, pieces::king, select::jumps, Position, Sequence>;

                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(BitSet(p.material().kings(Color)));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
