#pragma once
#include <dctl/color.hpp>
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/jump.hpp>             // Generate (jump specialization)
#include <dctl/successor/generate/push.hpp>             // Generate (push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Generate<ToMove, select::legal, IsReverse>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using Jump = Generate<ToMove, select::jump, IsReverse>;
                using Push = Generate<ToMove, select::push, IsReverse>;

                Jump{}(p, moves);
                if (moves.empty())
                        Push{}(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
