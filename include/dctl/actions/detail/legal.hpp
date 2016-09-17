#pragma once
#include <dctl/actions/detail/primary_fwd.hpp>  // Actions (primary template)
#include <dctl/actions/detail/jump.hpp>         // Actions (jump specialization)
#include <dctl/actions/detail/push.hpp>         // Actions (push specialization)
#include <dctl/actions/select/legal.hpp>        // legal
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/actions/select/push.hpp>         // push

namespace dctl {
namespace detail {

template<class Color, class DuplicatesPolicy, class Reverse>
class Actions<Color, select::legal, DuplicatesPolicy, Reverse>
{
        using Jump = Actions<Color, select::jump, DuplicatesPolicy, Reverse>;
        using Push = Actions<Color, select::push, DuplicatesPolicy, Reverse>;
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
                auto const num_actions = Jump{}.count(state);
                return num_actions ? num_actions : Push{}.count(state);
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                return Push{}.detect(state) || Jump{}.detect(state);
        }
};

}       // namespace detail
}       // namespace dctl
