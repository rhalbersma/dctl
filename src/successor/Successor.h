#pragma once
#include "../node/Stack.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

/*

The Successor class forms the <Context> in a <State Pattern>, with the 
StateInterface class as the <State> and the State class as the <ConcreteState>.

The Successor class delegates selecting the concrete State object to the
Dispatcher class, depending on the position's side to move and the availability 
of kings and men among the pieces.

*/

template
<
        typename Selection,
        typename Rules
>
class Successor
:
        private utils::nonconstructible // enforce static semantics
{
public:
        // interface
        template<typename Board> static void generate(const Position<Board>&, Stack&);
        template<typename Board> static int count(const Position<Board>&);
        template<typename Board> static bool detect(const Position<Board>&);

private:
        // implementation
        template<typename Board> static bool invariant(const Position<Board>&, int);
};

}       // namespace dctl

// include template definitions inside header
#include "Successor.hpp"
