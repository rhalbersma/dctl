#pragma once
#include "../utility/TemplateTricks.hpp"

namespace dctl {

template<typename> class Position;

/*

The Mobility class forms a <Context> in a <State Pattern>, with the 
StateInterface class as the <State> and the State class as the <ConcreteState>.

The Mobility class also forms a <Client> in a <FlyWeight Pattern>, 
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
class Mobility
:
        private utility::nonconstructible // enforce static semantics
{
public:
        // interface
        template<bool, typename Board> static int count(const Position<Board>&);
};

}       // namespace dctl

// include template definitions inside header
#include "Mobility.hpp"
