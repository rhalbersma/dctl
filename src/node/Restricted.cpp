#include <cassert>
#include "Restricted.h"
#include "Side.h"
#include "../utils/Bit.h"

namespace dctl {

KingMoves::KingMoves()
:
        king_(0),
        moves_(0)
{
        assert(invariant());
}

BitBoard KingMoves::king() const
{
        return king_;
}

PlyCount KingMoves::moves() const
{
        return moves_;
}

void KingMoves::reset()
{
        king_ = 0;
        moves_ = 0;

        assert(invariant());
}

void KingMoves::init(BitBoard dest)
{
        assert(bit::is_single(dest));

        king_ = dest;
        moves_ = 1;

        assert(invariant());
}

void KingMoves::increment(BitBoard dest)
{
        assert(bit::is_single(dest));

        king_= dest;
        ++moves_;

        assert(invariant());
}

bool KingMoves::invariant() const
{
        return (
                !bit::is_multiple(king()) &&
                (king() == 0) == (moves() == 0)
        );
}

}       // namespace dctl
