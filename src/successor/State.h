#pragma once
#include "StateInterface.h"
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successor {

/*

The State class forms the <ConcreteState> in a <State Pattern>, with the 
StateInterface class as the <State> and the Successor class as the <Context>.

The State class also forms the <AbstractClass> in a <Template Method Pattern>,
with the Driver class as the <ConcreteClass>.

*/

template<bool Color, int Material, typename Rules, typename Board> 
class State
: 
        public StateInterface<Rules, Board>
{
private:		
        virtual void do_generate_legal(const Position<Board>&, Stack&) const;
        virtual void do_generate_moves(const Position<Board>&, Stack&) const;
        virtual void do_generate_jumps(const Position<Board>&, Stack&) const;
        virtual void do_generate_reverse(const Position<Board>&, Stack&) const;
        virtual void do_generate_promotions(const Position<Board>&, Stack&) const;

        virtual int do_count_legal(const Position<Board>&) const;
        virtual int do_count_moves(const Position<Board>&) const;
        virtual int do_count_jumps(const Position<Board>&) const;
        virtual int do_count_reverse(const Position<Board>&) const;
        virtual int do_count_promotions(const Position<Board>&) const;

        virtual bool do_detect_legal(const Position<Board>&) const;
        virtual bool do_detect_moves(const Position<Board>&) const;
        virtual bool do_detect_jumps(const Position<Board>&) const;
        virtual bool do_detect_reverse(const Position<Board>&) const;
        virtual bool do_detect_promotions(const Position<Board>&) const;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "State.hpp"
