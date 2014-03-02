#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/all_jump.hpp>         // Generate (piece jumps specialization)
#include <dctl/successor/generate/all_push.hpp>         // Generate (piece moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jumps
#include <dctl/successor/select/push.hpp>               // moves

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<bool Color, class Pieces>
struct Generate<Color, Pieces, select::legal>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using AllJump = Generate<Color, Pieces, select::jump>;
                using AllPush = Generate<Color, Pieces, select::push>;

                AllJump{}(p, moves);
                if (moves.empty())
                        AllPush{}(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
