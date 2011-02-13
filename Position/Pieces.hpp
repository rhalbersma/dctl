// initialize with a set of bitboards
template<bool Color>
void Pieces::init(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	pieces_[Color] = active_pieces;
	pieces_[!Color] = opponent_pieces;
        kings_ = kings;
}
