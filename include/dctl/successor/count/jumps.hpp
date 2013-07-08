#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/copy/all_jumps.hpp>            // copy (piece jumps specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/move.hpp>                           // Move
#include <dctl/utility/stack_vector.hpp>                // Vector, Alloc, Arena

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for jumps
template<bool Color, class Pieces>
struct count<Color, Pieces, select::jumps>
{
        template<class Position>
        int operator()(Position const& p) const
        {
                Arena<Move> mar_;
                Alloc<Move> mal_(mar_);
                stack_vector<Move> moves_(mal_);

                copy<Color, Pieces, select::jumps>()(p, moves_);
                return static_cast<int>(moves_.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
