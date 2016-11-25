#pragma once
#include <dctl/actions/detail/primary_fwd.hpp>  // Actions (primary template)
#include <dctl/actions/detail/jump.hpp>         // Actions (jump specialization)
#include <dctl/actions/detail/push.hpp>         // Actions (push specialization)
#include <dctl/actions/select/legal.hpp>        // legal
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/actions/select/push.hpp>         // push
#include <dctl/color_piece.hpp>                 // color, color_

namespace dctl {
namespace detail {

#define cj

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
}       // namespace dctl
