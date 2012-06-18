#pragma once
#include "Dispatcher.hpp"
#include "../node/State.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {

/*

        For purposes of move generation, a position has 8 possible states,
        depending on the side to move and the availability of kings and pawns. 
        The move generator *dynamically* dispatches the state-specific routines.
        For efficiency, the states are *statically* stored in a FlyWeightFactory.

        State Design Pattern
        --------------------
        State                   : StateInterface
        ConcreteState           : State
        Context                 : Successor, Mobility <------------- this class

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility <------------- this class

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
        template<bool Color, template<typename, typename> class Position, typename Rules, typename Board>
        static int count(Position<Rules, Board> const& p)
        {
                typedef successor::Dispatcher<Selection, Position, Rules, Board> Delegate;
                return Delegate::select(state<Color>(p))->count(p);
        }
};

}       // namespace dctl
