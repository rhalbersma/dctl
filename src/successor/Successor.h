#pragma once
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

/*

The Successor class forms the <Context> in a <State Pattern>, with the 
StateInterface class as the <State> and the State class as the <ConcreteState>.

The Successor class dispatches a pointer to a State object depending on the 
position's side to move and the availability of kings and men among the pieces.

*/

template<typename Rules, typename Board>
class Successor
:
        private utils::nonconstructible // enforce static semantics
{
public:
        // interface
        static void generate_legal(const Position<Board>&, Stack&);
        static void generate_moves(const Position<Board>&, Stack&);
        static void generate_jumps(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static int count_legal(const Position<Board>&);
        static int count_moves(const Position<Board>&);
        static int count_jumps(const Position<Board>&);
        static int count_reverse(const Position<Board>&);
        static int count_promotions(const Position<Board>&);

        static bool detect_legal(const Position<Board>&);
        static bool detect_moves(const Position<Board>&);
        static bool detect_jumps(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

        template<bool> static int count_mobility(const Position<Board>&);

private:
        // implementation
        static bool invariant_legal(const Position<Board>&, int);
        static bool invariant_moves(const Position<Board>&, int);
        static bool invariant_jumps(const Position<Board>&, int);
        static bool invariant_reverse(const Position<Board>&, int);
        static bool invariant_promotions(const Position<Board>&, int);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);
};

}       // namespace dctl

// include template definitions inside header
#include "Successor.hpp"
