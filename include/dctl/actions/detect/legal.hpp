#pragma once
#include <dctl/color.hpp>                               // Player
#include <dctl/actions/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/actions/detect/jump.hpp>               // Detect (jump specialization)
#include <dctl/actions/detect/push.hpp>               // Detect (push specialization)
#include <dctl/actions/select/legal.hpp>              // legal
#include <dctl/actions/select/jump.hpp>               // jump
#include <dctl/actions/select/push.hpp>               // push

namespace dctl {
namespace actions {

template<Color ToMove, bool Reverse>
class Detect<ToMove, select::legal, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                using Push = Detect<ToMove, select::push, Reverse>;
                using Jump = Detect<ToMove, select::jump, Reverse>;

                return Push{}(p) || Jump{}(p);
        }
};

}       // namespace actions
}       // namespace dctl
