#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

/*

        The StateInterface class forms the <State> in a <State>
        Design Pattern, with the State class as the <ConcreteState>.
        Examples of the <Context> include the Successor and Mobility classes.

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility

*/

template<typename Position>
class StateInterface
{
public:
        // non-virtual interface

        void generate(const Position& p, Stack& moves) const
        {
                do_generate(p, moves);
                BOOST_ASSERT(invariant(p, moves.size()));
        }

        int count(const Position& p) const
        {
                return do_count(p);
        }

        bool detect(const Position& p) const
        {
                return do_detect(p);
        }

protected:
        // disable deletion of Derived* through Base* 
        // enable deletion of Base* through Derived*
        ~StateInterface()
        {
                // no-op
        }

private:
        // invariant relating generate, count and detect
        bool invariant(const Position& p, int number) const
        {
                return (
                        (count(p) == number) &&
                        (detect(p) == (number > 0))
                );
        }

        // pure virtual implementation

        virtual void do_generate(const Position&, Stack&) const = 0;
        virtual int do_count(const Position&) const = 0;
        virtual bool do_detect(const Position&) const = 0;
};

}       // namespace successor
}       // namespace dctl
