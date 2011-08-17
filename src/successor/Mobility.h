#pragma once
#include "../utils/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

/*

The Mobility class forms the <Context> in a <State Pattern>, with the 
StateInterface class as the <State> and the State class as the <ConcreteState>.

The Mobility class delegates selecting the concrete State object to the
Dispatcher class, depending on the position's side to move and the availability 
of kings and men among the pieces.

*/

template
<
        typename Selection,
        typename Rules
>
class Mobility
:
        private utils::nonconstructible // enforce static semantics
{
public:
        // interface
        template<bool, typename Board> static int count(const Position<Board>&);
};

}       // namespace dctl

// include template definitions inside header
#include "Mobility.hpp"
