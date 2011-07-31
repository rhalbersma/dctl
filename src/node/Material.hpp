namespace dctl {

// initialize with a set of bitboards
template<bool Color>
void Material::init(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	pieces_[ Color] = active_pieces;
	pieces_[!Color] = opponent_pieces;
        kings_ = kings;
}

// initialize with a set of bitboards
template<bool Color>
Material Material::create(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	Material temp;
        temp.init<Color>(active_pieces, opponent_pieces, kings);
        return temp;
}

}       // namespace dctl
