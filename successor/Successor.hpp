#pragma once
#include "Dispatcher.hpp"
#include "../node/Stack.hpp"
#include "../node/State.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

template<typename> struct Position;

/*

        The Successor class forms a <Context> in a <State> design pattern, 
        with the StateInterface class as the <State> and the State class 
        as the <ConcreteState>.

        The Successor class also forms a <Client> in a <FlyWeight> design
        pattern, with the StateInterface class as the <FlyWeight>, the 
        State class as the <ConcreteFlyWeight>, and the Dispatcher class 
        as the <FlyWeightFactory>. 

        A particular position's state is determined by its side to move 
        and the availability of kings and men among its pieces. For efficiency,
        all 8 unique position states are shared through the <FlyWeightFactory>.

*/

template
<
        typename Selection,
        typename Rules
>
class Successor
:
        private nonconstructible // enforce static semantics
{
public:
        // interface
        template<typename Board> 
        static void generate(const Position<Board>& p, Stack& moves)
        {
                successor::Dispatcher<Selection, Rules, Board>::select(state(p))->generate(p, moves);
        }
        
        template<typename Board> 
        static int count(const Position<Board>& p)
        {
                return successor::Dispatcher<Selection, Rules, Board>::select(state(p))->count(p);
        }
        
        template<typename Board> 
        static bool detect(const Position<Board>& p)
        {
                return successor::Dispatcher<Selection, Rules, Board>::select(state(p))->detect(p);
        }
};

}       // namespace dctl
