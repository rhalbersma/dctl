#pragma once
#include "GeneratePiece.h"
#include "../Position/Reversible/Pieces.h"

template<bool Color, typename Rules, typename Board> 
class GenerateFactory
{
public:
        static const AbstractGeneratePiece<Color, Rules, Board>* create(Pieces::Composition);

private:
        static const GeneratePiece<Color, Pieces::NONE, Rules, Board> PREFAB_NONE;
        static const GeneratePiece<Color, Pieces::PAWN, Rules, Board> PREFAB_PAWN;
        static const GeneratePiece<Color, Pieces::KING, Rules, Board> PREFAB_KING;
        static const GeneratePiece<Color, Pieces::BOTH, Rules, Board> PREFAB_BOTH;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "GenerateFactory.hpp"
