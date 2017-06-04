#pragma once
#include <dctl/core/actions/detail/primary_fwd.hpp>  // Actions (primary template)
#include <dctl/core/actions/detail/jump.hpp>         // Actions (jump specialization)
#include <dctl/core/actions/detail/push.hpp>         // Actions (push specialization)
#include <dctl/core/actions/select/legal.hpp>        // legal
#include <dctl/core/actions/select/jump.hpp>         // jump
#include <dctl/core/actions/select/push.hpp>         // push
#include <dctl/core/state/color_piece.hpp>                 // color, color_

namespace dctl {
namespace core {
namespace detail {

//#define cj

template<color Side, class DuplicatesPolicy, class Reverse>
class Actions<color_<Side>, select::legal, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
        using Jump = Actions<to_move_, select::jump, DuplicatesPolicy, Reverse>;
        using Push = Actions<to_move_, select::push, DuplicatesPolicy, Reverse>;

public:
        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& a) const
        {
#ifndef cj
                if (Jump{}.generate(s, a); a.empty()) {
                        Push{}.generate(s, a);
                }
#else
                if (Jump{}.detect(s)) {
                        Jump{}.generate(s, a);
                } else {
                        Push{}.generate(s, a);
                }
#endif
        }

        template<class State>
        auto count(State const& s) const
        {
                if (auto const num_jumps = Jump{}.count(s); !num_jumps) {
                        return Push{}.count(s);
                } else {
                        return num_jumps;
                }
        }

        template<class State>
        auto detect(State const& s) const noexcept
        {
                return Push{}.detect(s) || Jump{}.detect(s);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
