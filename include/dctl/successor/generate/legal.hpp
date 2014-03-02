#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // generate (primary template)
#include <dctl/successor/generate/all_jump.hpp>        // generate (piece jumps specialization)
#include <dctl/successor/generate/all_push.hpp>        // generate (piece moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>              // jumps
#include <dctl/successor/select/push.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, class Pieces>
struct generate<Color, Pieces, select::legal>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using DoJumps = generate<Color, Pieces, select::jump>;
                using DoMoves = generate<Color, Pieces, select::push>;

                DoJumps{}(p, moves);
                if (moves.empty())
                        DoMoves{}(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
