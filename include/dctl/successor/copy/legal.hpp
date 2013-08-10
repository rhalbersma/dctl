#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/all_jumps.hpp>            // copy (piece jumps specialization)
#include <dctl/successor/copy/all_moves.hpp>            // copy (piece moves specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
#include <dctl/successor/copy/king_moves.hpp>           // copy (king moves specialization)
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/copy/pawn_moves.hpp>           // copy (pawn moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, class Pieces>
struct copy<Color, Pieces, select::legal>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using DoJumps = copy<Color, Pieces, select::jumps>;
                using DoMoves = copy<Color, Pieces, select::moves>;

                DoJumps()(p, moves);
                if (moves.empty())
                        DoMoves()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
