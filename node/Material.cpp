#include <boost/assert.hpp>
#include <functional>
#include "Material.h"
#include "Side.hpp"
#include "../bit/Bit.hpp"

namespace dctl {

// zero initialize
Material::Material(BitBoard)
{
        init<Side::black>(0, 0, 0);
        BOOST_ASSERT(invariant());
}

// initialize with a set of bitboards
Material::Material(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings)
{
        init<Side::black>(black_pieces, white_pieces, kings);
        BOOST_ASSERT(invariant());
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
	return pieces(Side::black) ^ pieces(Side::white);
}

// xor-assign the set bits of another piece set
Material& Material::operator^=(const Material& other)
{
        pieces_[Side::black] ^= other.pieces(Side::black);
        pieces_[Side::white] ^= other.pieces(Side::white);
        kings_ ^= other.kings();

        BOOST_ASSERT(invariant());
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
		bit::is_exclusive(pieces(Side::black), pieces(Side::white)) &&
                bit::is_within(kings(), pieces())
        );
}

bool operator==(const Material& left, const Material& right)
{
        return (
		(left.pieces(Side::black) == right.pieces(Side::black)) &&
		(left.pieces(Side::white) == right.pieces(Side::white)) &&
                            (left.kings() == right.kings())
        );
}

bool operator!=(const Material& left, const Material& right)
{
        return !(left == right);
}

}       // namespace dctl
