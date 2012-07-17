#pragma once
#include "StateInterface.hpp"
#include "Driver.hpp"
#include "Result.hpp"
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

        // TODO: use C++11 template aliases
        template<typename Result>
        struct Delegate
        : 
                Driver<Color, Material, Selection, Result, Rules, Board>
        {};

        typedef Position<Rules, Board> Position;

        // virtual implemenation

        virtual void do_generate(Position const& p, Stack& moves) const
        {
                Delegate<generation>::generate(p, moves);
        }

        virtual int do_count(Position const& p) const
        {
                return Delegate<enumeration>::count(p);
        }

        virtual bool do_detect(Position const& p) const
        {
                return Delegate<detection>::detect(p);
        }
};

}       // namespace successor
}       // namespace dctl
