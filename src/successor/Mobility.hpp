#pragma once
#include "Dispatcher.hpp"
#include "../node/State.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {

/*

        The Mobility class forms a <Context> in a <State> Design Pattern,
        with the StateInterface class as the <State> and the State class
        as the <ConcreteState>.

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility

        A particular position's state is determined by its side to move
        and the availability of kings and pawns among its pieces.
        For efficiency, all 8 unique position states are shared through
        the <FlyWeightFactory>.

*/

template
<
        typename Selection
>
struct Mobility
:
        // enforce static semantics
        private nonconstructible
{
public:
        template<bool Color, typename Rules, typename Board, template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                typedef successor::Dispatcher<Selection, Rules, Board, Position> Delegate;
                return Delegate::select(state<Color>(p))->count(p);
        }
};

}       // namespace dctl
