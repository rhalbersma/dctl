template<bool Color, typename Rules, typename Board> 
const AbstractGenerateMaterial<Color, Rules, Board>* const MaterialSelector<Color, Rules, Board>::STRATEGY[] = {
        &STRATEGY_NONE, &STRATEGY_PAWN, &STRATEGY_KING, &STRATEGY_BOTH         
};

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::NONE, Rules, Board> MaterialSelector<Color, Rules, Board>::STRATEGY_NONE;

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::PAWN, Rules, Board> MaterialSelector<Color, Rules, Board>::STRATEGY_PAWN;

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::KING, Rules, Board> MaterialSelector<Color, Rules, Board>::STRATEGY_KING;

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::BOTH, Rules, Board> MaterialSelector<Color, Rules, Board>::STRATEGY_BOTH;

template<bool Color, typename Rules, typename Board>
const AbstractGenerateMaterial<Color, Rules, Board>* MaterialSelector<Color, Rules, Board>::select(Pieces::Composition material)
{
        return STRATEGY[material];
};
