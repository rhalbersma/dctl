#include <cassert>
#include <functional>
#include "Material.h"
#include "Side.h"
#include "../utils/Bit.h"

namespace dctl {

// zero initialize
Material::Material(BitBoard)
{
        init<Side::BLACK>(0, 0, 0);
        assert(invariant());
}

// initialize with a set of bitboards
Material::Material(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings)
{
        init<Side::BLACK>(black_pieces, white_pieces, kings);
        assert(invariant());
}

// black or white men
BitBoard Material::men(bool color) const
{
        return pieces(color) & ~kings();
}

// black or white kings
BitBoard Material::kings(bool color) const
{
        return pieces(color) & kings();
}

// black or white pieces
BitBoard Material::pieces(bool color) const
{
        return pieces_[color];
}

// black and white men
BitBoard Material::men() const
{
        return pieces() & ~kings();
}

// black and white kings
BitBoard Material::kings() const
{
        return kings_;
}

// black and white pieces
BitBoard Material::pieces() const
{
	return pieces(Side::BLACK) ^ pieces(Side::WHITE);
}

// xor-assign the set bits of another piece set
Material& Material::operator^=(const Material& other)
{
        pieces_[Side::BLACK] ^= other.pieces(Side::BLACK);
        pieces_[Side::WHITE] ^= other.pieces(Side::WHITE);
        kings_ ^= other.kings();

        assert(invariant());
        return *this;
}

// xor-assign the set bits of two piece sets
const Material operator^(const Material& left, const Material& right)
{
        return Material(left) ^= right;
}

// logical consistency of the representation
bool Material::invariant() const
{
        return (
		bit::is_exclusive(pieces(Side::BLACK), pieces(Side::WHITE)) &&
                bit::is_within(kings(), pieces())
        );
}

bool operator==(const Material& left, const Material& right)
{
        return (
		(left.pieces(Side::BLACK) == right.pieces(Side::BLACK)) &&
		(left.pieces(Side::WHITE) == right.pieces(Side::WHITE)) &&
                            (left.kings() == right.kings())
        );
}

bool operator!=(const Material& left, const Material& right)
{
        return !(left == right);
}

}       // namespace dctl
