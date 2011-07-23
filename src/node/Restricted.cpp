#include <cassert>
#include "Restricted.h"
#include "Side.h"
#include "../utils/Bit.h"

namespace dctl {

Restricted::Restricted()
{
        king_[Side::BLACK] = king_[Side::WHITE] = 0;
        moves_[Side::BLACK] = moves_[Side::WHITE] = 0;
        assert(invariant());
}

BitBoard Restricted::king(bool color) const
{
        return king_[color];
}

PlyCount Restricted::moves(bool color) const
{
        return moves_[color];
}

void Restricted::reset(bool color)
{
        king_[color] = 0;
        moves_[color] = 0;

        assert(invariant(color));
}

void Restricted::init(bool color, BitBoard dest)
{
        assert(bit::is_single(dest));

        king_[color] = dest;
        moves_[color] = 1;

        assert(invariant(color));
}

void Restricted::increment(bool color, BitBoard dest)
{
        assert(bit::is_single(dest));

        king_[color] = dest;
        ++moves_[color];

        assert(invariant(color));
}

bool Restricted::invariant() const
{
        return invariant(Side::BLACK) && invariant(Side::WHITE);
}

bool Restricted::invariant(bool color) const
{
        return (
                !bit::is_multiple(king(color)) &&
                bit::is_exclusive(king(color), king(!color)) &&
                (king_[color] == 0) == (moves_[color] == 0)
        );
}

}       // namespace dctl
