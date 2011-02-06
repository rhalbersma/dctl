template<bool Color, typename Rules, typename Board> 
const AbstractGenerateMaterial<Color, Rules, Board>* const MaterialSelector<Color, Rules, Board>::MATERIAL[] = {
        &MATERIAL_NONE, &MATERIAL_PAWN, &MATERIAL_KING, &MATERIAL_BOTH         
};

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::NONE, Rules, Board> MaterialSelector<Color, Rules, Board>::MATERIAL_NONE;

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::PAWN, Rules, Board> MaterialSelector<Color, Rules, Board>::MATERIAL_PAWN;

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::KING, Rules, Board> MaterialSelector<Color, Rules, Board>::MATERIAL_KING;

template<bool Color, typename Rules, typename Board> 
const GenerateMaterial<Color, Pieces::BOTH, Rules, Board> MaterialSelector<Color, Rules, Board>::MATERIAL_BOTH;

template<bool Color, typename Rules, typename Board>
const AbstractGenerateMaterial<Color, Rules, Board>* MaterialSelector<Color, Rules, Board>::select(Pieces::Composition material)
{
        return MATERIAL[material];
};
