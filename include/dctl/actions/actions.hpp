#pragma once
#include <dctl/actions/detail/specializations.hpp>      // Generate
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/color.hpp>                               // Color, black, white
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t
#include <type_traits>                                  // bool_constant

namespace dctl {
namespace core {

template<class Select = select::legal, bool KeepDuplicates = false, bool Reverse = false>
class Actions
{
        template<Color ToMove>
        using Impl = detail::Actions<ToMove, Select, std::bool_constant<KeepDuplicates>, std::bool_constant<Reverse>>;

        template<Color ToMove, class State>
        auto invariant(State const& state, std::size_t n) const
        {
                return
                        (count <ToMove>(state) ==  n     ) &&
                        (detect<ToMove>(state) == (n > 0))
                ;
        }

public:
        template<Color ToMove, class State, class Sequence>
        auto generate(State const& state, Sequence& actions) const
        {
                Impl<ToMove>{}.generate(state, actions);
                assert((invariant<ToMove>(state, actions.size())));
        }

        template<class State, class Sequence>
        auto generate(State const& state, Sequence& actions) const
        {
                return
                        (state.is_to_move(Color::black)) ?
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
                        (state.is_to_move(Color::black)) ?
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
                        (state.is_to_move(Color::black)) ?
                        detect<Color::black>(state) :
                        detect<Color::white>(state)
                ;
        }
};

}       // namespace core
}       // namespace dctl
