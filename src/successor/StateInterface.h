#pragma once
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successor {

/*

The StateInterface class forms the <State> in a <State Pattern>, with the 
State class as the <ConcreteState>. Examples of the <Context> include the
Successor and Mobility classes.

The StateInterface class also forms the <FlyWeight> in a <FlyWeight Pattern>, 
with the State class as the <ConcreteFlyWeight>, and the Dispatcher class as 
the <FlyWeightFactory>.

*/

template<typename Board> 
class StateInterface
{
public:
        // non-virtual interface
        void generate(const Position<Board>&, Stack&) const;
        int count(const Position<Board>&) const;
        bool detect(const Position<Board>&) const;

protected:
        // destructor
        ~StateInterface() {};

private:
        // pure virtual implementation
        virtual void do_generate(const Position<Board>&, Stack&) const = 0;
        virtual int do_count(const Position<Board>&) const = 0;
        virtual bool do_detect(const Position<Board>&) const = 0;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "StateInterface.hpp"
