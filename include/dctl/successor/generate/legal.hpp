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

template<Color ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Generate<ToMove, select::legal, RemoveDuplicateJumps, Reverse>
{
public:
        template<class Position, class Sequence>
        auto operator()(Position const& p, Sequence& moves) const
        {
                using Jump = Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>;
                using Push = Generate<ToMove, select::push, false               , Reverse>;

                Jump{}(p, moves);
                if (moves.empty())
                        Push{}(p, moves);
        }
};

}       // namespace successor
}       // namespace dctl
