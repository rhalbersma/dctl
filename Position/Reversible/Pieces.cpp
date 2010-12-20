#include "Pieces.h"
#include "Side.h"
#include "../../Utilities/Bit.h"
#include <cassert>

// zero initialize
Pieces::Pieces(BitBoard)
{
        init<Side::BLACK>(0, 0, 0);
        assert(invariant());
}

// initialize with a set of bitboards
Pieces::Pieces(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings)
{
        init<Side::BLACK>(black_pieces, white_pieces, kings);
        assert(invariant());
}

// overloaded equality operator
bool Pieces::operator==(const Pieces& other) const
{
        return (
		(pieces(Side::BLACK) == other.pieces(Side::BLACK)) &&
		(pieces(Side::WHITE) == other.pieces(Side::WHITE)) &&
                            (kings() == other.kings())
        );
}

// overloaded inequality operator
bool Pieces::operator!=(const Pieces& other) const
{
        return !(*this == other);
}

// black or white men
BitBoard Pieces::men(bool color) const
{
        return pieces(color) & ~kings();
}

// black or white kings
BitBoard Pieces::kings(bool color) const
{
        return pieces(color) & kings();
}

// black or white pieces
BitBoard Pieces::pieces(bool color) const
{
        return d_pieces[color];
}

// black and white men
BitBoard Pieces::men(void) const
{
        return occupied() & ~kings();
}

// black and white kings
BitBoard Pieces::kings(void) const
{
        return d_kings;
}

// black and white pieces
BitBoard Pieces::occupied(void) const
{
	return pieces(Side::BLACK) ^ pieces(Side::WHITE);
}

// composition of black or white pieces
Pieces::Composition Pieces::composition(bool color) const
{
        return static_cast<Composition>(((kings(color) != 0) << 1) + (men(color) != 0));
}

// toggle the set bits of another piece set
Pieces& Pieces::toggle(const Pieces& other)
{
        d_pieces[Side::BLACK] ^= other.pieces(Side::BLACK);
        d_pieces[Side::WHITE] ^= other.pieces(Side::WHITE);
        d_kings ^= other.kings();

        assert(invariant());
        return *this;
}

// logical consistency of the representation
bool Pieces::invariant(void) const
{
        return (
		Bit::is_exclusive(pieces(Side::BLACK), pieces(Side::WHITE)) &&
                Bit::is_within(kings(), occupied())
        );
}
