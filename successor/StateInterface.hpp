#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "../node/Stack.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

/*

        The StateInterface class forms the <State> in a <State>
        design pattern, with the State class as the <ConcreteState>. 
        Examples of the <Context> include the Successor and Mobility classes.

        The StateInterface class also forms the <FlyWeight> in a <FlyWeight>
        design pattern, with the State class as the <ConcreteFlyWeight>, and 
        the Dispatcher class as the <FlyWeightFactory>.

*/

template<typename Board> 
class StateInterface
{
public:
        // non-virtual interface
        void generate(const Position<Board>& p, Stack& moves) const 
        {
                do_generate(p, moves);
                BOOST_ASSERT(invariant(p, moves.size()));
        }

        int count(const Position<Board>& p) const
        {
                return do_count(p);
        }

        bool detect(const Position<Board>& p) const 
        { 
                return do_detect(p);
        }

protected:
        // non-virtual destructor
        ~StateInterface() 
        { 
                /* no-op */ 
        }

private:
        // invariant relating generate, count and detect
        bool invariant(const Position<Board>& p, int number) const
        {
                return (
                        (count(p) == number) && 
                        (detect(p) == (number > 0))
                );
        }

        // pure virtual implementation
        virtual void do_generate(const Position<Board>&, Stack&) const = 0;
        virtual int do_count(const Position<Board>&) const = 0;
        virtual bool do_detect(const Position<Board>&) const = 0;
};

}       // namespace successor
}       // namespace dctl
