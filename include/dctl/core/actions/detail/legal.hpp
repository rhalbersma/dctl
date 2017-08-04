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

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::legal, DuplicatesPolicy, Reverse>
{
        using jumps = actions<color_<Side>, select::jump, DuplicatesPolicy, Reverse>;
        using moves = actions<color_<Side>, select::push, DuplicatesPolicy, Reverse>;
public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return moves::detect(s) || jumps::detect(s);
        }

        template<class State>
        static auto count(State const& s)
        {
                if (auto const num_jumps = jumps::count(s); num_jumps != 0) {
                        return num_jumps;
                } else {
                        return moves::count(s);
                }
        }

        template<class State, class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                if (jumps::generate(s, seq); seq.empty()) {
                        moves::generate(s, seq);
                }
        }
};

}       // namespace detail
}       // namespace dctl::core
