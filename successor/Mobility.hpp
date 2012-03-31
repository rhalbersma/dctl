#pragma once
#include "Dispatcher.hpp"
#include "../node/Position_fwd.hpp"
#include "../node/State.hpp"
#include "../utility/NonConstructible.hpp"

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
        typename Selection,
        typename Rules
>
struct Mobility
:
        private nonconstructible        // enforce static semantics
{
        template<bool Color, typename Board>
        static int count(const Position<Board>& p)
        {
                return successor::Dispatcher<Selection, Rules, Board>::select(state<Color>(p))->count(p);
        }
};

}       // namespace dctl
