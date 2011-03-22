#pragma once
#include "Template.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
namespace Move { class Stack; }

template<bool Color, typename Rules, typename Board> 
class Template<Color, Pieces::BOTH, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Move::Stack&);
        static size_t count(const Position<Board>&);        
        static bool detect(const Position<Board>&);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothMoves.hpp"
