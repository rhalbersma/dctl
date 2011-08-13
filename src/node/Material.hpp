namespace dctl {

// create a new struct from a set of BitBoards
template<bool Color>
Material Material::create(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	Material temp;
        temp.init<Color>(active_pieces, opponent_pieces, kings);
        return temp;
}

// initialize with a set of BitBoards
template<bool Color>
void Material::init(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	pieces_[ Color] = active_pieces;
	pieces_[!Color] = opponent_pieces;
        kings_ = kings;
}

}       // namespace dctl
