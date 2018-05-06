#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/detail/builder.hpp>           // builder
#include <dctl/core/model/container.hpp>
#include <dctl/core/model/detail/king_jumps.hpp>        // king_jumps
#include <dctl/core/model/detail/pawn_jumps.hpp>        // pawn_jumps
#include <dctl/core/action/basic_action.hpp>            // action
#include <dctl/core/state/color_piece.hpp>              // color, color_, kings_, pawn_
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/type_traits.hpp>                    // rules_t, board_t
#include <cassert>                                      // assert
#include <type_traits>

namespace dctl::core::model {
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

        template<class... Args>
        auto emplace_back(Args const&...)
        {
                ++m_count;
        }

        template<class... Args>
        auto emplace_back(Args&&...)
        {
                ++m_count;
        }
};

template<class...>
class jumps;

template<color Side, class DuplicatesPolicy>
class jumps<color_<Side>, DuplicatesPolicy>
{
        using to_move_ = color_<Side>;
        template<class State> using king_jumps = detail::king_jumps<rules_t<State>, board_t<State>, to_move_>;
        template<class State> using pawn_jumps = detail::pawn_jumps<rules_t<State>, board_t<State>, to_move_>;

        template<class State, class Action>
        using container_type = std::conditional_t<
                is_trivial_precedence_v<rules_t<State>> && std::is_same_v<DuplicatesPolicy, keep_duplicates_tag>,
                counter_container<Action>,
                default_container<Action>
        >;
public:
        template<class State>
        static auto detect(State const& s) noexcept
        {
                return
                        pawn_jumps<State>::detect(s) ||
                        king_jumps<State>::detect(s)
                ;
        }

        template<class State>
        static auto count(State const& s)
        {
                container_type<State, basic_action<rules_t<State>, board_t<State>>> seq;
                generate(s, seq);
                return static_cast<int>(seq.size());
        }

        template<class State, class SequenceContainer = default_container<basic_action<rules_t<State>, board_t<State>>>>
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
                king_jumps<State>::generate(b);
                pawn_jumps<State>::generate(b);
        }
};

}       // namespace detail
}       // namespace dctl::core::model
