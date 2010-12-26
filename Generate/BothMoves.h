#pragma once
#include "GenerateDriver.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool Color, typename Rules, typename Board> 
class GenerateDriver<Color, Pieces::BOTH, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Propagate<Rules, Board>&);
        static void generate_promotions(const Position<Board>&, Propagate<Rules, Board>&);

        static size_t count(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothMoves.hpp"
