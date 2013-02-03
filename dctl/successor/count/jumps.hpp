#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/successor/copy/both_jumps.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for jumps enumeration
template<bool Color, int Material, typename Position>
struct count<Color, Material, select::jumps, Position>
{
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
