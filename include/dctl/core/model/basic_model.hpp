#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/container.hpp>
#include <dctl/core/model/detail/jumps.hpp>     // jumps
#include <dctl/core/model/detail/moves.hpp>     // moves
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/core/state/color.hpp>            // black, white
#include <dctl/util/type_traits.hpp>            // value_t
#include <cassert>                              // assert
#include <concepts>                             // same_as
#include <type_traits>                          // false_type

namespace dctl::core {

template
<
        class DuplicatesPolicy = drop_duplicates_tag,
        class ReverseGenerator = std::false_type
>
class basic_model
{
        template<class Color> using jumps = detail::jumps<Color, DuplicatesPolicy>;
        template<class Color> using moves = detail::moves<Color, ReverseGenerator>;

        template<class Color>
        static constexpr auto assert_invariants(auto const& state [[maybe_unused]], auto const& actions [[maybe_unused]]) noexcept
        {
                assert(count<Color>(state) == static_cast<int>(actions.size()) && detect<Color>(state) == (actions.size() > 0));
        }

public:
        [[nodiscard]] static constexpr auto detect(auto const& state) noexcept
        {
                return state.to_move() == color::black ? detect<black_>(state) : detect<white_>(state);
        }

        template<class Color>
        [[nodiscard]] static constexpr auto detect(auto const& state) noexcept
        {
                return moves<Color>::detect(state) || jumps<Color>::detect(state);
        }

        [[nodiscard]] static constexpr auto count(auto const& state) noexcept
        {
                return state.to_move() == color::black ? count<black_>(state) : count<white_>(state);
        }

        template<class Color>
        [[nodiscard]] static constexpr auto count(auto const& state) noexcept
        {
                if (auto const num_jumps = jumps<Color>::count(state); num_jumps != 0) {
                        return num_jumps;
                } else {
                        return moves<Color>::count(state);
                }
        }

        template<class State, class SequenceContainer = default_container<basic_action<rules_t<State>, board_t<State>>>>
        [[nodiscard]] static constexpr auto generate(State const& state) noexcept
        {
                SequenceContainer actions;
                generate(state, actions);
                return actions;
        }

        template<class Color, class State, class SequenceContainer = default_container<basic_action<rules_t<State>, board_t<State>>>>
        [[nodiscard]] static constexpr auto generate(State const& state) noexcept
        {
                SequenceContainer actions;
                generate<Color>(state, actions);
                return actions;
        }

        [[nodiscard]] static constexpr auto generate(auto const& state, auto& actions) noexcept
        {
                return state.to_move() == color::black ? generate<black_>(state, actions) : generate<white_>(state, actions);
        }

        template<class Color>
        [[nodiscard]] static constexpr auto generate(auto const& state, auto& actions) noexcept
        {
                if (jumps<Color>::generate(state, actions); actions.empty()) {
                        moves<Color>::generate(state, actions);
                }
                assert_invariants<Color>(state, actions);
        }
};

inline constexpr auto keep_duplicates_gen = basic_model<keep_duplicates_tag>();
inline constexpr auto drop_duplicates_gen = basic_model<drop_duplicates_tag>();

}       // namespace dctl::core
