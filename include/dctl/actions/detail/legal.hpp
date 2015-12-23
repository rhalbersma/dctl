#pragma once
#include <dctl/actions/detail/primary_fwd.hpp>  // Actions (primary template)
#include <dctl/actions/detail/jump.hpp>         // Actions (jump specialization)
#include <dctl/actions/detail/push.hpp>         // Actions (push specialization)
#include <dctl/actions/select/legal.hpp>        // legal
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/actions/select/push.hpp>         // push
#include <dctl/color.hpp>                       // Color

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class Unique, class Reverse>
class Actions<ToMove, select::legal, Unique, Reverse>
{
        using Jump = Actions<ToMove, select::jump, Unique, Reverse>;
        using Push = Actions<ToMove, select::push, Unique, Reverse>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                Jump{}.generate(state, actions);
                if (actions.empty())
                        Push{}.generate(state, actions);
        }

        template<class State>
        auto count(State const& state) const
        {
                auto num_actions = Jump{}.count(state);
                if (!num_actions)
                        num_actions += Push{}.count(state);
                return num_actions;
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                return Push{}.detect(state) || Jump{}.detect(state);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
