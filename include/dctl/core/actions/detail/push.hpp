#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/king_move.hpp>       // king_move
#include <dctl/core/actions/detail/pawn_move.hpp>       // pawn_move
#include <dctl/core/actions/detail/primary_fwd.hpp>     // actions (primary template)
#include <dctl/core/actions/select/push.hpp>            // push
#include <dctl/core/state/color_piece.hpp>              // color_

namespace dctl::core {
namespace detail {

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::push, DuplicatesPolicy, Reverse>
{
        template<class State> using king_move = detail::king_move<color_<Side>, Reverse, State>;
        template<class State> using pawn_move = detail::pawn_move<color_<Side>, Reverse, State>;
public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return pawn_move<State>::detect(s) || king_move<State>::detect(s);
        }

        template<class State>
        static auto count(State const& s) noexcept
        {
                return king_move<State>::count(s) + pawn_move<State>::count(s);
        }

        template<class State, class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                king_move<State>::generate(s, seq);
                pawn_move<State>::generate(s, seq);
        }
};

}       // namespace detail
}       // namespace dctl::core
