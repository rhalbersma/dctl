#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::NONE, Rules, Board> GenerateFactory<Color, Rules, Board>::PREFAB_NONE;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::PAWN, Rules, Board> GenerateFactory<Color, Rules, Board>::PREFAB_PAWN;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::KING, Rules, Board> GenerateFactory<Color, Rules, Board>::PREFAB_KING;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::BOTH, Rules, Board> GenerateFactory<Color, Rules, Board>::PREFAB_BOTH;

template<bool Color, typename Rules, typename Board> FORCE_INLINE
const AbstractGeneratePiece<Color, Rules, Board>* GenerateFactory<Color, Rules, Board>::create(Pieces::Composition pc)
{                        
        switch(pc) {
        case Pieces::NONE:
		return &PREFAB_NONE;
        case Pieces::PAWN:
                return &PREFAB_PAWN;
        case Pieces::KING:
                return &PREFAB_KING;
        case Pieces::BOTH:
                return &PREFAB_BOTH;
        default:
                assert(false);
                return 0;
        }
};
