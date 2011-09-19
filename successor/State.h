#pragma once
#include "Selection.h"
#include "StateInterface.hpp"
#include "../node/Stack.hpp"

namespace dctl {

template<typename> class Position;

namespace successor {

/*

The State class forms the <ConcreteState> in a <State Pattern>, with the 
StateInterface class as the <State>. Examples of the <Context> include the
Successor and Mobility classes.

The State class also forms the <ConcreteFlyWeight> in a <FlyWeight Pattern>, 
with the StateInterface class as the <FlyWeight>, and the Dispatcher class 
as the <FlyWeightFactory>. Examples of the <Client> include the Successor and
Mobility classes.

The State class also forms the <AbstractClass> in a <Template Method Pattern>,
with the Driver class as the <ConcreteClass>.

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
        virtual void do_generate(const Position<Board>&, Stack&) const;
        virtual int do_count(const Position<Board>&) const;
        virtual bool do_detect(const Position<Board>&) const;
};

template<bool Color, int Material, typename Rules, typename Board> 
class State<Color, Material, Legal, Rules, Board>
: 
        public StateInterface<Board>
{
private:		
        virtual void do_generate(const Position<Board>&, Stack&) const;
        virtual int do_count(const Position<Board>&) const;
        virtual bool do_detect(const Position<Board>&) const;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "State.hpp"
