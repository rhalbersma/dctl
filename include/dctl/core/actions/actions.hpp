#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/specializations.hpp> // generate
#include <dctl/core/actions/select/legal.hpp>           // legal
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/core/state/color_piece.hpp>              // black, white
#include <dctl/util/type_traits.hpp>                    // value_t
#include <boost/container/static_vector.hpp>
#include <cassert>                                      // assert
#include <type_traits>                                  // bool_constant, is_same_v

namespace dctl::core {

template<class Select = select::legal, class DuplicatesPolicy = drop_duplicates_tag, bool Reverse = false>
class actions
{
        template<class Color>
        constexpr static auto impl = detail::actions<Color, Select, DuplicatesPolicy, std::bool_constant<Reverse>>{};

        template<class Color, class State, class SequenceContainer>
        auto assert_invariants(State const& s [[maybe_unused]], SequenceContainer const& seq [[maybe_unused]]) const noexcept
        {
                auto const n [[maybe_unused]] = static_cast<int>(seq.size());
                assert( count<Color>(s) ==  n     );
                assert(detect<Color>(s) == (n > 0));
        }

public:
        template<class State, class SequenceContainer = boost::container::static_vector<action<rules_t<State>, board_t<State>>, 128>>
        auto generate(State const& s) const
        {
                SequenceContainer seq;
                generate(s, seq);
                return seq;
        }

        template<class Color, class State, class SequenceContainer = boost::container::static_vector<action<rules_t<State>, board_t<State>>, 128>>
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
                impl<Color>.generate(s, seq);
                assert_invariants<Color>(s, seq);
        }

        template<class State>
        auto count(State const& s) const
        {
                return s.to_move() == color::black ? count<black_>(s) : count<white_>(s);
        }

        template<class Color, class State>
        auto count(State const& s) const
        {
                return impl<Color>.count(s);
        }

        template<class State>
        auto detect(State const& s) const
        {
                return s.to_move() == color::black ? detect<black_>(s) : detect<white_>(s);
        }

        template<class Color, class State>
        auto detect(State const& s) const
        {
                return impl<Color>.detect(s);
        }
};

constexpr auto keep_duplicates_gen = actions<select::legal, keep_duplicates_tag>{};
constexpr auto drop_duplicates_gen = actions<select::legal, drop_duplicates_tag>{};

}       // namespace dctl::core
