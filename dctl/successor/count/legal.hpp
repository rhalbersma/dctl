#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/jumps.hpp>               // count (jumps specialization)
#include <dctl/successor/count/both_moves.hpp>          // count (combined king and pawn moves specialization)
#include <dctl/successor/count/king_moves.hpp>          // count (king moves specialization)
#include <dctl/successor/count/pawn_moves.hpp>          // count (pawn moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material>
struct count<Color, Material, select::legal>
{
        template<typename Position>
        int operator()(Position const& p)
        {
                typedef count<Color, Material, select::jumps> DoJumps;
                typedef count<Color, Material, select::moves> DoMoves;

                auto num_moves = DoJumps()(p);
                if (!num_moves)
                        num_moves += DoMoves()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
