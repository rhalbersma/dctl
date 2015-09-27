#pragma once
#include <dctl/actions/count/primary_fwd.hpp>   // Count (primary template)
#include <dctl/actions/count/jump.hpp>          // Count (jump specialization)
#include <dctl/actions/count/push.hpp>          // Count (push specialization)
#include <dctl/actions/select/legal.hpp>        // legal
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/actions/select/push.hpp>         // push
#include <dctl/color.hpp>                       // Color

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Count<ToMove, select::legal, Unique, Reverse>
{
public:
        template<class State>
        auto operator()(State const& state) const
        {
                using Jump = Count<ToMove, select::jump, Unique, Reverse>;
                using Push = Count<ToMove, select::push, Unique, Reverse>;

                auto num_moves = Jump{}(state);
                if (!num_moves)
                        num_moves += Push{}(state);
                return num_moves;
        }
};

}       // namespace actions
}       // namespace dctl
