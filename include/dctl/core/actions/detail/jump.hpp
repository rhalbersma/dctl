#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/action/action.hpp>                  // action
#include <dctl/core/actions/detail/builder.hpp>         // builder
#include <dctl/core/actions/detail/king_jump.hpp>       // king_jump
#include <dctl/core/actions/detail/pawn_jump.hpp>       // pawn_jump
#include <dctl/core/actions/detail/primary_fwd.hpp>     // actions (primary template)
#include <dctl/core/actions/select/jump.hpp>            // jump
#include <dctl/core/state/color_piece.hpp>              // color, color_, kings_, pawn_
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/type_traits.hpp>                    // rules_t, board_t
#include <boost/container/static_vector.hpp>
#include <cassert>                                      // assert
#include <type_traits>

namespace dctl::core {
namespace detail {

class MoveCounter
{
        int n{};
public:
        MoveCounter() = default;

        auto& operator++()
        {
                ++n; return *this;
        }

        auto size() const
        {
                return n;
        }
};

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::jump, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
        template<class State> using king_jump = detail::king_jump<to_move_, Reverse, State>;
        template<class State> using pawn_jump = detail::pawn_jump<to_move_, Reverse, State>;
public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return pawn_jump<State>::detect(s) || king_jump<State>::detect(s);
        }

        template<class State>
        static auto count(State const& s)
        {
                return static_cast<int>(generate(s).size());
        }

        template<class State, class SequenceContainer = boost::container::static_vector<action<rules_t<State>, board_t<State>>, 128>>
        static auto generate(State const& s)
        {
                SequenceContainer seq;
                generate(s, seq);
                return seq;
        }

        template<class State, class SequenceContainer>
        static auto generate(State const& s, SequenceContainer& seq)
        {
                auto b = builder<to_move_, DuplicatesPolicy, State, SequenceContainer>{s, seq};
                king_jump<State>::generate(b);
                pawn_jump<State>::generate(b);
        }
};

}       // namespace detail
}       // namespace dctl::core
