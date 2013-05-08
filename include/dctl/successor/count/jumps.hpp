#pragma once
#include <cstddef>                                      // size_t
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/copy/piece_jumps.hpp>          // copy (piece jumps specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/move.hpp>                           // Move
#include <dctl/utility/stack_vector.hpp>                          // Vector, Alloc, Arena

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for jumps
template<bool Color, typename Material>
struct count<Color, Material, select::jumps>
{
        template<typename Position>
        std::size_t operator()(Position const& p) const
        {
                Arena<Move>::type mar_;
                Alloc<Move>::type mal_(mar_);
                stack_vector<Move>::type moves_(mal_);

                copy<Color, Material, select::jumps>()(p, moves_);
                return moves_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
