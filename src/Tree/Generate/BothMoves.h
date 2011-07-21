#pragma once
#include "Driver.h"
#include "../node/Stack.h"
#include "../../utils/IntegerTypes.h"

namespace tree {

template<typename> class Position;

namespace generate {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Pieces::BOTH, MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static size_t count(const Position<Board>&);
        static size_t count_reverse(const Position<Board>&);
        static size_t count_promotions(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothMoves.hpp"
