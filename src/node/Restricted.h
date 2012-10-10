#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

class KingMoves
{
public:
        // constructors
        KingMoves();

        // views
        BitBoard king() const;
        PlyCount moves() const;

        // modifiers
        void reset();
        void init(BitBoard);
        void increment(BitBoard);

private:
        // implementation
        bool invariant() const;

        // representation
        BitBoard king_;
        PlyCount moves_;
};

typedef KingMoves Restricted[2];

// predicates
template<typename> bool is_max(PlyCount);

}       // namespace dctl

// include template definitions inside header
#include "Restricted.hpp"
