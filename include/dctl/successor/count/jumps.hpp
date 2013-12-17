#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/generate/all_jumps.hpp>        // geneate (piece jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/move/move.hpp>                           // Move
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
                using R = typename Position::rules_type;
                using B = typename Position::board_type;
                Arena<Move<R,B> > a;
                stack_vector<Move<R,B> > moves_(Alloc<Move<R,B> >{a});

                generate<Color, Pieces, select::jumps>{}(p, moves_);
                return static_cast<int>(moves_.size());
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
