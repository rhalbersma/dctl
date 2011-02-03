#pragma once
#include "GenerateMaterial.h"
#include "../Position/Pieces.h"

template<bool Color, typename Rules, typename Board> 
class MaterialSelector
{
public:
        static const AbstractGenerateMaterial<Color, Rules, Board>* select(Pieces::Composition);

private:
        static const AbstractGenerateMaterial<Color, Rules, Board>* const STRATEGY[];

        static const GenerateMaterial<Color, Pieces::NONE, Rules, Board> STRATEGY_NONE;
        static const GenerateMaterial<Color, Pieces::PAWN, Rules, Board> STRATEGY_PAWN;
        static const GenerateMaterial<Color, Pieces::KING, Rules, Board> STRATEGY_KING;
        static const GenerateMaterial<Color, Pieces::BOTH, Rules, Board> STRATEGY_BOTH;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "MaterialSelector.hpp"
