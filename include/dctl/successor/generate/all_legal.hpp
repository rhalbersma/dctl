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

template<Color ToMove, bool IsReverse>
class Generate<ToMove, IsReverse, select::legal>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using Jump = Generate<ToMove, IsReverse, select::jump>;
                using Push = Generate<ToMove, IsReverse, select::push>;

                Jump{}(p, moves);
                if (moves.empty())
                        Push{}(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
