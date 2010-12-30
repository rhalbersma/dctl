#pragma once
#include "GeneratePiece.h"
#include "../Position/Reversible/Pieces.h"

template<bool Color, typename Rules, typename Board> 
class GenerateFactory
{
public:
        static const AbstractGeneratePiece<Color, Rules, Board>* select_generator(Pieces::Composition);

private:
        static const AbstractGeneratePiece<Color, Rules, Board>* d_generator[];

        static const GeneratePiece<Color, Pieces::NONE, Rules, Board> GENERATE_NONE;
        static const GeneratePiece<Color, Pieces::PAWN, Rules, Board> GENERATE_PAWN;
        static const GeneratePiece<Color, Pieces::KING, Rules, Board> GENERATE_KING;
        static const GeneratePiece<Color, Pieces::BOTH, Rules, Board> GENERATE_BOTH;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "GenerateFactory.hpp"
