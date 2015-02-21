#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/jump.hpp>               // Detect (jump specialization)
#include <dctl/successor/detect/push.hpp>               // Detect (push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

template<Color ToMove, bool Reverse>
class Detect<ToMove, select::legal, Reverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using Push = Detect<ToMove, select::push, Reverse>;
                using Jump = Detect<ToMove, select::jump, Reverse>;

                return Push{}(p) || Jump{}(p);
        }
};

}       // namespace successor
}       // namespace dctl
