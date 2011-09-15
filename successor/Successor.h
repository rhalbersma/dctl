#pragma once
#include "../node/Stack.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

template<typename> class Position;

/*

The Successor class forms a <Context> in a <State Pattern>, with the 
StateInterface class as the <State> and the State class as the <ConcreteState>.

The Successor class also forms a <Client> in a <FlyWeight Pattern>, 
with the StateInterface class as the <FlyWeight>, the State class as the 
<ConcreteFlyWeight>, and the Dispatcher class as the <FlyWeightFactory>. 

A particular position's state is determined by its side to move and the 
availability of kings and men among its pieces. For efficiency, all 8 unique 
position states are shared through the <FlyWeightFactory>.

*/

template
<
        typename Selection,
        typename Rules
>
class Successor
:
        private utility::nonconstructible // enforce static semantics
{
public:
        // interface
        template<typename Board> static void generate(const Position<Board>&, Stack&);
        template<typename Board> static int count(const Position<Board>&);
        template<typename Board> static bool detect(const Position<Board>&);
};

}       // namespace dctl

// include template definitions inside header
#include "Successor.hpp"
