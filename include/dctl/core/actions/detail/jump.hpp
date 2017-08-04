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

template<class Action>
class counter_container
{
        Action m_action {};
        int m_count = 0;
public:
        using value_type = Action;

        counter_container() = default;

        auto& back() const
        {
                return m_action;
        }

        auto empty() const
        {
                return m_count == 0;
        }

        auto size() const
        {
                return m_count;
        }

        auto push_back(Action const&)
        {
                ++m_count;
        }

        auto push_back(Action&&)
        {
                ++m_count;
        }
};

template<color Side, class DuplicatesPolicy, class Reverse>
class actions<color_<Side>, select::jump, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
        template<class State> using king_jump = detail::king_jump<to_move_, Reverse, State>;
        template<class State> using pawn_jump = detail::pawn_jump<to_move_, Reverse, State>;

        template<class State, class Action>
        using container_type = std::conditional_t<
                is_trivial_precedence_v<rules_t<State>> && std::is_same_v<DuplicatesPolicy, keep_duplicates_tag>,
                counter_container<Action>,
                boost::container::static_vector<Action, 128>
        >;
public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return pawn_jump<State>::detect(s) || king_jump<State>::detect(s);
        }

        template<class State>
        static auto count(State const& s)
        {
                container_type<State, action<rules_t<State>, board_t<State>>> seq;
                generate(s, seq);
                return static_cast<int>(seq.size());
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
