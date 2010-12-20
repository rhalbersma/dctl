#pragma once
#include "GeneratePiece.h"
#include "../Position/Reversible/Pieces.h"

template<bool Color, typename Rules, typename Board> 
class GenerateFactory
{
public:
        static const AbstractGeneratePiece<Color, Rules, Board>* create(Pieces::Composition);

private:
        static const GeneratePiece<Pieces::NONE, Color, Rules, Board> PREFAB_NONE;
        static const GeneratePiece<Pieces::PAWN, Color, Rules, Board> PREFAB_PAWN;
        static const GeneratePiece<Pieces::KING, Color, Rules, Board> PREFAB_KING;
        static const GeneratePiece<Pieces::BOTH, Color, Rules, Board> PREFAB_BOTH;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "GenerateFactory.hpp"
