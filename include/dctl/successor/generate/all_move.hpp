#pragma once
#include <dctl/color.hpp>
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/all_jump.hpp>         // Generate (piece jumps specialization)
#include <dctl/successor/generate/all_push.hpp>         // Generate (piece moves specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jumps
#include <dctl/successor/select/push.hpp>               // moves

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<Color ToMove, class Pieces>
class Generate<ToMove, Pieces, select::legal>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using AllJump = Generate<ToMove, Pieces, select::jump>;
                using AllPush = Generate<ToMove, Pieces, select::push>;

                AllJump{}(p, moves);
                if (moves.empty())
                        AllPush{}(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
