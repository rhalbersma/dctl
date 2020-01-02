#pragma once

//          Copyright Rein Halbersma 2010-2020.
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
#include <type_traits>                          // false_type, is_same_v

namespace dctl::core::model {

template<class DuplicatesPolicy = drop_duplicates_tag, class ReverseGenerator = std::false_type>
class actions
{
        template<class Color> using jumps = detail::jumps<Color, DuplicatesPolicy>;
        template<class Color> using moves = detail::moves<Color, ReverseGenerator>;

        template<class Color, class State, class SequenceContainer>
        auto assert_invariants(State const& s [[maybe_unused]], SequenceContainer const& seq [[maybe_unused]]) const noexcept
        {
                auto const n [[maybe_unused]] = static_cast<int>(seq.size());
                assert( count<Color>(s) ==  n     );
                assert(detect<Color>(s) == (n > 0));
        }

public:
        template<class State>
        auto detect(State const& s) const
        {
                return s.to_move() == color::black ? detect<black_>(s) : detect<white_>(s);
        }

        template<class Color, class State>
        auto detect(State const& s) const
        {
                return moves<Color>::detect(s) || jumps<Color>::detect(s);
        }

        template<class State>
        auto count(State const& s) const
        {
                return s.to_move() == color::black ? count<black_>(s) : count<white_>(s);
        }

        template<class Color, class State>
        auto count(State const& s) const
        {
                if (auto const num_jumps = jumps<Color>::count(s); num_jumps != 0) {
                        return num_jumps;
                } else {
                        return moves<Color>::count(s);
                }
        }

        template<class State, class SequenceContainer = default_container<basic_action<rules_t<State>, board_t<State>>>>
        auto generate(State const& s) const
        {
                SequenceContainer seq;
                generate(s, seq);
                return seq;
        }

        template<class Color, class State, class SequenceContainer = default_container<basic_action<rules_t<State>, board_t<State>>>>
        auto generate(State const& s) const
        {
                SequenceContainer seq;
                generate<Color>(s, seq);
                return seq;
        }

        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& seq) const
        {
                return s.to_move() == color::black ? generate<black_>(s, seq) : generate<white_>(s, seq);
        }

        template<class Color, class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& seq) const
        {
                using action_type = value_t<SequenceContainer>;
                static_assert(std::is_same_v<rules_t<State>, rules_t<action_type>>);
                static_assert(std::is_same_v<board_t<State>, board_t<action_type>>);
                if (jumps<Color>::generate(s, seq); seq.empty()) {
                        moves<Color>::generate(s, seq);
                }
                assert_invariants<Color>(s, seq);
        }
};

constexpr auto keep_duplicates_gen = actions<keep_duplicates_tag>{};
constexpr auto drop_duplicates_gen = actions<drop_duplicates_tag>{};

}       // namespace dctl::core::model
