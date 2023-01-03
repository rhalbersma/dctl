#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/detail/builder.hpp>           // builder
#include <dctl/core/model/container.hpp>
#include <dctl/core/model/detail/king_jumps.hpp>        // king_jumps
#include <dctl/core/model/detail/pawn_jumps.hpp>        // pawn_jumps
#include <dctl/core/action/basic_action.hpp>            // action
#include <dctl/core/state/color.hpp>                    // color, color_
#include <dctl/core/state/piece.hpp>                    // pawn_, king_
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/type_traits.hpp>                    // rules_t, board_t
#include <cassert>                                      // assert
#include <concepts>                                     // same_as

namespace dctl::core::detail {

template<class Action>
class counter_container
{
        Action m_action {};
        int m_count = 0;
public:
        using value_type = Action;

        counter_container() = default;

        constexpr auto& back() const
        {
                return m_action;
        }

        constexpr auto empty() const
        {
                return m_count == 0;
        }

        constexpr auto size() const
        {
                return m_count;
        }

        constexpr auto push_back(Action const&)
        {
                ++m_count;
        }

        constexpr auto push_back(Action&&)
        {
                ++m_count;
        }

        constexpr auto emplace_back(auto&&...)
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

        template<class State>
        static constexpr bool MSVC_WORKAROUND = is_trivial_precedence_v<rules_t<State>> && std::same_as<DuplicatesPolicy, keep_duplicates_tag>;

        template<class State, class Action>
        using container_type = std::conditional_t<
                MSVC_WORKAROUND<State>,
                counter_container<Action>,
                default_container<Action>
        >;
public:
        template<class State>
        [[nodiscard]] static constexpr auto detect(State const& state) noexcept
        {
                return
                        pawn_jumps<State>::detect(state.pieces(color_c<Side>, pawn_c), state.targets(color_c<Side>, pawn_c), state.pieces(empty_c)) ||
                        king_jumps<State>::detect(state.pieces(color_c<Side>, king_c), state.targets(color_c<Side>, king_c), state.pieces(empty_c))
                ;
        }

        template<class State>
        [[nodiscard]] static constexpr auto count(State const& state) noexcept
        {
                container_type<State, basic_action<rules_t<State>, board_t<State>>> actions;
                generate(state, actions);
                return static_cast<int>(actions.size());
        }

        template<class State, class SequenceContainer = default_container<basic_action<rules_t<State>, board_t<State>>>>
        [[nodiscard]] static constexpr auto generate(State const& state) noexcept
        {
                SequenceContainer actions;
                generate(state, actions);
                return actions;
        }

        template<class State, class SequenceContainer>
        static constexpr auto generate(State const& state, SequenceContainer& actions) noexcept
        {
                auto b = builder<to_move_, DuplicatesPolicy, State, SequenceContainer>{state, actions};
                king_jumps<State>::generate(b);
                pawn_jumps<State>::generate(b);
        }
};

}       // namespace dctl::core::detail
