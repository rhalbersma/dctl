#include <cassert>

// initialize with a set of bitboards
template<bool Color>
void Pieces::init(BitBoard active_pieces, BitBoard opponent_pieces, BitBoard kings)
{
	d_pieces[Color] = active_pieces;
	d_pieces[!Color] = opponent_pieces;
        d_kings = kings;
}
