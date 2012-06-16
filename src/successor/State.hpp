#pragma once
#include "StateInterface.hpp"
#include "Driver.hpp"
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

/*

        The State class forms the <ConcreteState> in a <State>
        Design Pattern, with the StateInterface class as the <State>.
        Examples of the <Context> include the Successor and Mobility classes.

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
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
        typedef Position<Rules, Board> PositionType;

        // virtual implemenation

        virtual void do_generate(PositionType const& p, Stack& moves) const
        {
                Delegate::generate(p, moves);
        }

        virtual int do_count(PositionType const& p) const
        {
                return Delegate::count(p);
        }

        virtual bool do_detect(PositionType const& p) const
        {
                return Delegate::detect(p);
        }
};

}       // namespace successor
}       // namespace dctl
