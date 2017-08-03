#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/primary_fwd.hpp>     // actions (primary template)
#include <dctl/core/actions/detail/jump.hpp>            // actions (jump specialization)
#include <dctl/core/actions/detail/push.hpp>            // actions (push specialization)
#include <dctl/core/actions/select/legal.hpp>           // legal
#include <dctl/core/actions/select/jump.hpp>            // jump
#include <dctl/core/actions/select/push.hpp>            // push
#include <dctl/core/state/color_piece.hpp>              // color, color_

namespace dctl::core {
namespace detail {

//#define cj

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::legal, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
        using Jump = actions<to_move_, select::jump, DuplicatesPolicy, Reverse>;
        using Push = actions<to_move_, select::push, DuplicatesPolicy, Reverse>;

public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return Push::detect(s) || Jump::detect(s);
        }

        template<class State>
        static auto count(State const& s)
        {
                if (auto const num_jumps = Jump::count(s); !num_jumps) {
                        return Push::count(s);
                } else {
                        return num_jumps;
                }
        }

        template<class State, class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
#ifndef cj
                if (Jump::generate(s, seq); seq.empty()) {
                        Push::generate(s, seq);
                }
#else
                if (Jump::detect(s)) {
                        Jump::generate(s, seq);
                } else {
                        Push::generate(s, seq);
                }
#endif
        }
};

}       // namespace detail
}       // namespace dctl::core
