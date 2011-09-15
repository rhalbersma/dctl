#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "../bit/Bit.h"
#include "../rules/Rules.h"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

class KingMoves
{
public:
        // constructors
        KingMoves()
        :
                king_(0),
                moves_(0)
        {
                BOOST_ASSERT(invariant());
        }

        // views
        BitBoard king() const
        {
                return king_;
        }

        PlyCount moves() const
        {
                return moves_;
        }

        // modifiers
        void reset()
        {
                king_ = 0;
                moves_ = 0;

                BOOST_ASSERT(invariant());
        }

        void init(BitBoard dest)
        {
                BOOST_ASSERT(bit::is_single(dest));

                king_ = dest;
                moves_ = 1;

                BOOST_ASSERT(invariant());
        }

        void increment(BitBoard dest)
        {
                BOOST_ASSERT(bit::is_single(dest));

                king_= dest;
                ++moves_;

                BOOST_ASSERT(invariant());
        }

private:
        // implementation
        bool invariant() const
        {
                return (
                        !bit::is_multiple(king()) &&
                        (king() == 0) == (moves() == 0)
                );
        }

        // representation
        BitBoard king_;
        PlyCount moves_;
};

typedef KingMoves Restricted[2];

// predicates
template<typename Rules>
bool is_max(PlyCount moves)
{
        return moves == rules::max_same_king_moves<Rules>::value;
}

}       // namespace dctl
