#include <boost/assert.hpp>
#include "Restricted.h"
#include "Side.h"
#include "../bit/Bit.h"

namespace dctl {

KingMoves::KingMoves()
:
        king_(0),
        moves_(0)
{
        BOOST_ASSERT(invariant());
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

        BOOST_ASSERT(invariant());
}

void KingMoves::init(BitBoard dest)
{
        BOOST_ASSERT(bit::is_single(dest));

        king_ = dest;
        moves_ = 1;

        BOOST_ASSERT(invariant());
}

void KingMoves::increment(BitBoard dest)
{
        BOOST_ASSERT(bit::is_single(dest));

        king_= dest;
        ++moves_;

        BOOST_ASSERT(invariant());
}

bool KingMoves::invariant() const
{
        return (
                !bit::is_multiple(king()) &&
                (king() == 0) == (moves() == 0)
        );
}

}       // namespace dctl
