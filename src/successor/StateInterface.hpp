#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/utility.hpp>            // noncopyable
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
        State                   : StateInterface <------------------ this class
        ConcreteState           : State
        Context                 : Successor, Mobility

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface <------------------ this class
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility

*/

template
<
        typename Position
>
class StateInterface
:
        // enforce reference semantics
        private boost::noncopyable
{
public:
        // non-virtual interface

        void generate(Position const& p, Stack& moves) const
        {
                do_generate(p, moves);
                BOOST_ASSERT(invariant(p, moves.size()));
        }

        int count(Position const& p) const
        {
                return do_count(p);
        }

        bool detect(Position const& p) const
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
        bool invariant(Position const& p, int number) const
        {
                return (
                        (count(p) == number) &&
                        (detect(p) == (number > 0))
                );
        }

        // pure virtual implementation

        virtual void do_generate(Position const&, Stack&) const = 0;
        virtual int do_count(Position const&) const = 0;
        virtual bool do_detect(Position const&) const = 0;
};

}       // namespace successor
}       // namespace dctl
