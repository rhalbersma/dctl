#pragma once
#include "../../Containers/Generic/VectorBucket.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/Ply.h"

class SameKingMoves
{
public:
        // constructors
        SameKingMoves(void) {};

        // predicates
        template<PlyCount> bool is_restricted(void) const;

        // element access
        BitBoard& king(void);
        BitBoard king(void) const;
        PlyCount& moves(void);
        PlyCount moves(void) const;

        // modifiers
        void copy_back(void);
        void push_back(BitBoard, PlyCount);
        void pop_back(void);
        void init(BitBoard);
        void increment(BitBoard);
        void reset(void);

private:
        // implementation
        bool invariant(void) const;

        // representation
        VectorBucket<BitBoard, PLY_MAX> d_king;
        VectorBucket<PlyCount, PLY_MAX> d_moves;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "SameKingMoves.hpp"
