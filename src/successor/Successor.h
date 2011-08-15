#pragma once
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
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

class Legal {};

template<typename Rules, typename Board, typename Selection>
class Successor
:
        private utils::nonconstructible // enforce static semantics
{
public:
        // interface
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        template<bool> static int count_mobility(const Position<Board>&);

private:
        // implementation
        static bool invariant(const Position<Board>&, int);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);
};

}       // namespace dctl

// include template definitions inside header
#include "Successor.hpp"
