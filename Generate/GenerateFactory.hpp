template<bool Color, typename Rules, typename Board> 
const AbstractGeneratePiece<Color, Rules, Board>* GenerateFactory<Color, Rules, Board>::d_generator[] = {
        &GENERATE_NONE, &GENERATE_PAWN, &GENERATE_KING, &GENERATE_BOTH         
};

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::NONE, Rules, Board> GenerateFactory<Color, Rules, Board>::GENERATE_NONE;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::PAWN, Rules, Board> GenerateFactory<Color, Rules, Board>::GENERATE_PAWN;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::KING, Rules, Board> GenerateFactory<Color, Rules, Board>::GENERATE_KING;

template<bool Color, typename Rules, typename Board> 
const GeneratePiece<Color, Pieces::BOTH, Rules, Board> GenerateFactory<Color, Rules, Board>::GENERATE_BOTH;

template<bool Color, typename Rules, typename Board>
const AbstractGeneratePiece<Color, Rules, Board>* GenerateFactory<Color, Rules, Board>::select_generator(Pieces::Composition pc)
{
        return d_generator[pc];
};
