#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/king_push.hpp>       // king_push
#include <dctl/core/actions/detail/pawn_push.hpp>       // pawn_push
#include <dctl/core/actions/select/push.hpp>            // push
#include <dctl/core/state/color_piece.hpp>              // color, color_, kings_, pawn_

namespace dctl::core {
namespace detail {

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::push, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
        template<class State> using king_push = detail::king_push<to_move_, Reverse, State>;
        template<class State> using pawn_push = detail::pawn_push<to_move_, Reverse, State>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& seq) const
        {
                return king_push<State>{}.generate(s, seq) , pawn_push<State>{}.generate(s, seq);
        }

        template<class State>
        auto count(State const& s) const noexcept
        {
                return king_push<State>{}.count(s) + pawn_push<State>{}.count(s);
        }

        template<class State>
        auto detect(State const& s) const noexcept
        {
                return pawn_push<State>{}.detect(s) || king_push<State>{}.detect(s);
        }
};

}       // namespace detail
}       // namespace dctl::core
