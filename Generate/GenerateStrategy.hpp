template<bool Color, typename Rules, typename Board> 
const AbstractGeneratePiece<Color, Rules, Board>* const GenerateStrategy<Color, Rules, Board>::STRATEGY[] = {
        &GENERATE_NONE, &GENERATE_PAWN, &GENERATE_KING, &GENERATE_BOTH         
};

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::NONE, Rules, Board> GenerateStrategy<Color, Rules, Board>::GENERATE_NONE;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::PAWN, Rules, Board> GenerateStrategy<Color, Rules, Board>::GENERATE_PAWN;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::KING, Rules, Board> GenerateStrategy<Color, Rules, Board>::GENERATE_KING;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::BOTH, Rules, Board> GenerateStrategy<Color, Rules, Board>::GENERATE_BOTH;

template<bool Color, typename Rules, typename Board>
const AbstractGeneratePiece<Color, Rules, Board>* GenerateStrategy<Color, Rules, Board>::select_strategy(Pieces::Composition pc)
{
        return STRATEGY[pc];
};
