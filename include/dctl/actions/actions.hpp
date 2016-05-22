#pragma once
#include <dctl/actions/detail/specializations.hpp>      // Generate
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/rule_traits.hpp>
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t
#include <type_traits>                                  // bool_constant

namespace dctl {
namespace core {

template<class Select = select::legal, class DuplicatesPolicy = drop_duplicates_tag, bool Reverse = false>
class Actions
{
        template<Color ToMove>
        using Impl = detail::Actions<ToMove, Select, DuplicatesPolicy, std::bool_constant<Reverse>>;

        template<Color ToMove, class State>
        auto assert_invariants(State const& state, std::size_t const n) const
        {
                assert(count <ToMove>(state) ==  n);
                assert(detect<ToMove>(state) == (n > 0));
        }

public:
        template<Color ToMove, class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                Impl<ToMove>{}.generate(state, actions);
                assert_invariants<ToMove>(state, actions.size());
        }

        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                return
                        state.is_to_move(Color::black) ?
                        generate<Color::black>(state, actions) :
                        generate<Color::white>(state, actions)
                ;
        }

        template<Color ToMove, class State>
        auto count(State const& state) const
        {
                return Impl<ToMove>{}.count(state);
        }

        template<class State>
        auto count(State const& state) const
        {
                return
                        state.is_to_move(Color::black) ?
                        count<Color::black>(state) :
                        count<Color::white>(state)
                ;
        }

        template<Color ToMove, class State>
        auto detect(State const& state) const
        {
                return Impl<ToMove>{}.detect(state);
        }

        template<class State>
        auto detect(State const& state) const
        {
                return
                        state.is_to_move(Color::black) ?
                        detect<Color::black>(state) :
                        detect<Color::white>(state)
                ;
        }
};

}       // namespace core
}       // namespace dctl
