#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/bit/Bit.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {

struct KingMoves
{
public:
        // structors
        KingMoves()
        :
                king_(0),
                moves_(0)
        {
                BOOST_ASSERT(invariant());
        }

        // queries
        BitBoard king() const
        {
                return (king_);
        }

        PlyCount moves() const
        {
                return (moves_);
        }

        // modifiers
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

        void reset()
        {
                BOOST_ASSERT(true);
                king_ = 0;
                moves_ = 0;
                BOOST_ASSERT(invariant());
        }

private:
        // implementation
        bool invariant() const
        {
                return (
                        !bit::is_multiple(king()) &&
                        (!king() == !moves())
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
        return (moves == Rules::max_same_king_moves::value);
}

}       // namespace dctl
