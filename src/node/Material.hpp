namespace dctl {

// initialize with a set of bitboards
template<bool Color>
void Material::init(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	pieces_[ Color] = active_pieces;
	pieces_[!Color] = opponent_pieces;
        kings_ = kings;
}

}       // namespace dctl
