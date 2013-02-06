#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/copy/both_jumps.hpp>           // copy (combined king and pawn jumps specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/move.hpp>                           // Move
#include <dctl/node/stack.hpp>                          // Vector, Alloc, Arena

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for jumps
template<bool Color, int Material>
struct count<Color, Material, select::jumps>
{
        template<typename Position>
        int operator()(Position const& p) const
        {
                Arena<Move> mar_;
                Alloc<Move> mal_(mar_);
                Vector<Move> moves_(mal_);

                copy<Color, Material, select::jumps>()(p, moves_);
                return moves_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
