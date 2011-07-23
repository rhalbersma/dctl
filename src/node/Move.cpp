#include "Move.h"

namespace dctl {

const Material& Move::material() const
{
        return material_;
}

bool Move::to_move() const
{
        return to_move_;
}

// black or white men
BitBoard Move::men(bool color) const
{
        return material_.men(color);
}

// black or white kings
BitBoard Move::kings(bool color) const
{
        return material_.kings(color);
}

// black or white pieces
BitBoard Move::pieces(bool color) const
{
        return material_.pieces(color);
}

// black and white men
BitBoard Move::men() const
{
        return material_.men();
}

// black and white kings
BitBoard Move::kings() const
{
        return material_.kings();
}

// black and white pieces
BitBoard Move::pieces() const
{
	return material_.pieces();
}

// logical consistency of the representation
bool Move::invariant() const
{
        return material_.invariant();
}

bool operator==(const Move& left, const Move& right)
{
        return left.material() == right.material();
}

bool operator!=(const Move& left, const Move& right)
{
        return !(left == right);
}

}       // namespace dctl
