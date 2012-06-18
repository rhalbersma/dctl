#pragma once
#include "StateInterface.hpp"
#include "Driver.hpp"
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

/*

        For purposes of move generation, a position has 8 possible states,
        depending on the side to move and the availability of kings and pawns. 
        The move generator *dynamically* dispatches the state-specific routines.
        For efficiency, the states are *statically* stored in a FlyWeightFactory.

        State Design Pattern
        --------------------
        State                   : StateInterface
        ConcreteState           : State <--------------------------- this class
        Context                 : Successor, Mobility

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State <--------------------------- this class
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility

*/

template
<
        bool Color,
        int Material,
        typename Selection,
        template<typename, typename> class Position,
        typename Rules,
        typename Board
>
class State
:
        public StateInterface< Position<Rules, Board> >
{
private:
        // typedefs

        typedef Driver<Color, Material, Selection, Rules, Board> Delegate;
        typedef Position<Rules, Board> Position;

        // virtual implemenation

        virtual void do_generate(Position const& p, Stack& moves) const
        {
                Delegate::generate(p, moves);
        }

        virtual int do_count(Position const& p) const
        {
                return Delegate::count(p);
        }

        virtual bool do_detect(Position const& p) const
        {
                return Delegate::detect(p);
        }
};

}       // namespace successor
}       // namespace dctl
