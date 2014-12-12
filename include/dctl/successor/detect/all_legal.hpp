#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/all_jump.hpp>           // Detect (piece jump specialization)
#include <dctl/successor/detect/all_push.hpp>           // Detect (piece push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Detect<ToMove, IsReverse, select::legal>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using Push = Detect<ToMove, IsReverse, select::push>;
                using Jump = Detect<ToMove, IsReverse, select::jump>;

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                // SPECULATE: #pushes > #jumps, so that the logical OR is more likely to short-circuit
                return Push{}(p) || Jump{}(p);
        }
};

}       // namespace successor
}       // namespace dctl
