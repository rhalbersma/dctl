#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

class Restricted
{
public:
        // constructors
        Restricted();

        // views
        BitBoard king(bool) const;
        PlyCount moves(bool) const;

        // modifiers
        void reset(bool);
        void init(bool, BitBoard);
        void increment(bool, BitBoard);

private:
        // implementation
        bool invariant() const;
        bool invariant(bool) const;

        // representation
        BitBoard king_[2];
        PlyCount moves_[2];
};

// predicates
template<typename> bool is_max(PlyCount);

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Restricted.hpp"
