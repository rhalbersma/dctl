#pragma once
#include <dctl/actions/detail/primary_fwd.hpp>  // Actions (primary template)
#include <dctl/actions/detail/jump.hpp>         // Actions (jump specialization)
#include <dctl/actions/detail/push.hpp>         // Actions (push specialization)
#include <dctl/actions/select/legal.hpp>        // legal
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/actions/select/push.hpp>         // push
#include <dctl/color_piece.hpp>                 // Color, color_constant

namespace dctl {
namespace detail {

template<Color Side, class DuplicatesPolicy, class Reverse>
class Actions<color_constant<Side>, select::legal, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_constant<Side>;
        using Jump = Actions<to_move_, select::jump, DuplicatesPolicy, Reverse>;
        using Push = Actions<to_move_, select::push, DuplicatesPolicy, Reverse>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                if (Jump{}.generate(state, actions); actions.empty()) {
                        Push{}.generate(state, actions);
                }
        }

        template<class State>
        auto count(State const& state) const
        {
                if (auto const num_actions = Jump{}.count(state); !num_actions) {
                        return Push{}.count(state);
                } else {
                        return num_actions;
                }
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                return Push{}.detect(state) || Jump{}.detect(state);
        }
};

}       // namespace detail
}       // namespace dctl
