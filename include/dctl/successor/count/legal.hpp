#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/jumps.hpp>               // count (jumps specialization)
#include <dctl/successor/count/all_moves.hpp>         // count (piece moves specialization)
#include <dctl/successor/count/king_moves.hpp>          // count (king moves specialization)
#include <dctl/successor/count/pawn_moves.hpp>          // count (pawn moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, class Pieces>
struct count<Color, Pieces, select::legal>
{
        template<class Position>
        int operator()(Position const& p)
        {
                typedef count<Color, Pieces, select::jumps> DoJumps;
                typedef count<Color, Pieces, select::moves> DoMoves;

                auto num_moves = DoJumps()(p);
                if (!num_moves)
                        num_moves += DoMoves()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
