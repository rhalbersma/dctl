#pragma once
#include <dctl/actions/detail/specializations.hpp>      // Generate
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/color_piece.hpp>                               // black, white
#include <dctl/rule_traits.hpp>
#include <dctl/utility/type_traits.hpp>                 // value_t
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t
#include <type_traits>                                  // bool_constant, is_same

namespace dctl {

template<class Select = select::legal, class DuplicatesPolicy = drop_duplicates_tag, bool Reverse = false>
class Actions
{
        template<class Color>
        using Impl = detail::Actions<Color, Select, DuplicatesPolicy, std::bool_constant<Reverse>>;

        template<class Color, class State, class SequenceContainer>
        auto assert_invariants([[maybe_unused]] State const& state, [[maybe_unused]] SequenceContainer const& actions) const
        {
                [[maybe_unused]] auto const n = actions.size();
                assert( count<Color>(state) ==  n     );
                assert(detect<Color>(state) == (n > 0));
        }

public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                return
                        state.is_to_move(Color::black) ?
                        generate<black_>(state, actions) :
                        generate<white_>(state, actions)
                ;
        }

        template<class Color, class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                using action_type = value_t<SequenceContainer>;
                static_assert(std::is_same<rules_t<State>, rules_t<action_type>>{});
                static_assert(std::is_same<board_t<State>, board_t<action_type>>{});
                Impl<Color>{}.generate(state, actions);
                assert_invariants<Color>(state, actions);
        }

        template<class State>
        auto count(State const& state) const
        {
                return
                        state.is_to_move(Color::black) ?
                        count<black_>(state) :
                        count<white_>(state)
                ;
        }

        template<class Color, class State>
        auto count(State const& state) const
        {
                return Impl<Color>{}.count(state);
        }

        template<class State>
        auto detect(State const& state) const
        {
                return
                        state.is_to_move(Color::black) ?
                        detect<black_>(state) :
                        detect<white_>(state)
                ;
        }

        template<class Color, class State>
        auto detect(State const& state) const
        {
                return Impl<Color>{}.detect(state);
        }
};

}       // namespace dctl
