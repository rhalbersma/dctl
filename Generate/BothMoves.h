#pragma once
#include "GenerateTemplate.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
class Move::List;

template<bool Color, typename Rules, typename Board> 
class GenerateTemplate<Color, Pieces::BOTH, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Move::List&);
        static size_t count(const Position<Board>&);        
        static bool detect(const Position<Board>&);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothMoves.hpp"
