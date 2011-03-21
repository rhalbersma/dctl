#include "Pieces.h"
#include "Side.h"
#include "../Utilities/Bit.h"
#include <cassert>
#include <functional>

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

// black and white men
BitBoard Pieces::men(void) const
{
        return occupied() & ~kings();
}

// black and white kings
BitBoard Pieces::kings(void) const
{
        return kings_;
}

// black and white pieces
BitBoard Pieces::occupied(void) const
{
	return pieces(Side::BLACK) ^ pieces(Side::WHITE);
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
        return pieces_[color];
}

// xor-assign the set bits of another piece set
Pieces& Pieces::operator^=(const Pieces& other)
{
        pieces_[Side::BLACK] ^= other.pieces(Side::BLACK);
        pieces_[Side::WHITE] ^= other.pieces(Side::WHITE);
        kings_ ^= other.kings();

        assert(invariant());
        return *this;
}

// xor-assign the set bits of two piece sets
Pieces operator^(const Pieces& left, const Pieces& right)
{
        return Pieces(left) ^= right;
}

// logical consistency of the representation
bool Pieces::invariant(void) const
{
        return (
		Bit::is_exclusive(pieces(Side::BLACK), pieces(Side::WHITE)) &&
                Bit::is_within(kings(), occupied())
        );
}
