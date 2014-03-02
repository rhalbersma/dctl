#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/jump.hpp>               // count (jumps specialization)
#include <dctl/successor/count/all_push.hpp>         // count (piece moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>              // jumps
#include <dctl/successor/select/push.hpp>              // moves

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
                using DoJumps = count<Color, Pieces, select::jump>;
                using DoMoves = count<Color, Pieces, select::push>;

                auto num_moves = DoJumps{}(p);
                if (!num_moves)
                        num_moves += DoMoves{}(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
