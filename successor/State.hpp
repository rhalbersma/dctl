#pragma once
#include "StateInterface.hpp"
#include "Driver.hpp"
#include "../node/Position_fwd.hpp"
#include "../node/Stack_fwd.hpp"

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
        typename Rules,
        typename Board
>
class State
:
        public StateInterface<Board>
{
private:
        virtual void do_generate(const Position<Board>& p, Stack& moves) const
        {
                Driver<Color, Material, Selection, Rules, Board>::generate(p, moves);
        }

        virtual int do_count(const Position<Board>& p) const
        {
                return Driver<Color, Material, Selection, Rules, Board>::count(p);
        }

        virtual bool do_detect(const Position<Board>& p) const
        {
                return Driver<Color, Material, Selection, Rules, Board>::detect(p);
        }
};

}       // namespace successor
}       // namespace dctl
