#pragma once
#include <dctl/actions/generate/specializations.hpp>    // Generate
#include <dctl/actions/count/specializations.hpp>       // Count
#include <dctl/actions/detect/specializations.hpp>      // Detect
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/color.hpp>                               // Color, black, white
#include <type_traits>                                  // bool_constant

namespace dctl {
namespace actions {

template<class Select = select::legal, bool Unique = true, bool Reverse = false>
class Successor
{
        template<Color ToMove, class State>
        auto invariant(State const& s, std::size_t n)
        {
                return
                        (count <ToMove>(s) ==  n     ) &&
                        (detect<ToMove>(s) == (n > 0))
                ;
        }

public:
        template<Color ToMove, class State, class Sequence>
        auto generate(State const& s, Sequence& moves)
        {
                Generate<ToMove, Select, std::bool_constant<Unique>, std::bool_constant<Reverse>>{}(s, moves);
                assert((invariant<ToMove>(s, moves.size())));
        }

        template<class State, class Sequence>
        auto generate(State const& s, Sequence& moves)
        {
                return
                        (s.is_to_move(Color::black)) ?
                        generate<Color::black>(s, moves) :
                        generate<Color::white>(s, moves)
                ;
        }

        template<Color ToMove, class State>
        auto count(State const& s)
        {
                return Count<ToMove, Select, std::bool_constant<Unique>, std::bool_constant<Reverse>>{}(s);
        }

        template<class State>
        auto count(State const& s)
        {
                return
                        (s.is_to_move(Color::black)) ?
                        count<Color::black>(s) :
                        count<Color::white>(s)
                ;
        }

        template<Color ToMove, class State>
        auto detect(State const& s)
        {
                return Detect<ToMove, Select, std::bool_constant<Reverse>>{}(s);
        }

        template<class State>
        auto detect(State const& s)
        {
                return
                        (s.is_to_move(Color::black)) ?
                        detect<Color::black>(s) :
                        detect<Color::white>(s)
                ;
        }
};

}       // namespace actions
}       // namespace dctl
