#pragma once
#include <dctl/actions/detail/specializations.hpp>      // Generate
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/color.hpp>                               // color, black, white
#include <dctl/rule_traits.hpp>
#include <dctl/utility/type_traits.hpp>
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t
#include <experimental/type_traits>                     // is_same
#include <type_traits>                                  // bool_constant

namespace dctl {
namespace core {

template<class Select = select::legal, class DuplicatesPolicy = drop_duplicates_tag, bool Reverse = false>
class Actions
{
        template<color ToMove>
        using Impl = detail::Actions<ToMove, Select, DuplicatesPolicy, std::bool_constant<Reverse>>;

        template<color ToMove, class State>
        auto assert_invariants(State const& state, std::size_t const n) const
        {
                assert(count <ToMove>(state) ==  n);
                assert(detect<ToMove>(state) == (n > 0));
        }

public:
        template<color ToMove, class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                using action_type = xstd::value_t<SequenceContainer>;
                static_assert(std::experimental::is_same_v<rules_t<State>, rules_t<action_type>>);
                static_assert(std::experimental::is_same_v<board_t<State>, board_t<action_type>>);
                Impl<ToMove>{}.generate(state, actions);
                assert_invariants<ToMove>(state, actions.size());
        }

        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                return
                        state.is_to_move(color::black) ?
                        generate<color::black>(state, actions) :
                        generate<color::white>(state, actions)
                ;
        }

        template<color ToMove, class State>
        auto count(State const& state) const
        {
                return Impl<ToMove>{}.count(state);
        }

        template<class State>
        auto count(State const& state) const
        {
                return
                        state.is_to_move(color::black) ?
                        count<color::black>(state) :
                        count<color::white>(state)
                ;
        }

        template<color ToMove, class State>
        auto detect(State const& state) const
        {
                return Impl<ToMove>{}.detect(state);
        }

        template<class State>
        auto detect(State const& state) const
        {
                return
                        state.is_to_move(color::black) ?
                        detect<color::black>(state) :
                        detect<color::white>(state)
                ;
        }
};

}       // namespace core
}       // namespace dctl
