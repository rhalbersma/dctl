#pragma once
#include "../../Containers/Generic/VectorBucket.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/Ply.h"

class NonConversion
{
public:
        // constructors
        NonConversion(void) {};

        // element access
        PlyCount& moves(void);
        PlyCount moves(void) const;

        // modifiers
        void copy_back(void);
        void push_back(PlyCount);
        void pop_back(void);
        void increment(void);
        void reset(void);

private:
        // representation
        VectorBucket<PlyCount, PLY_MAX> moves_;
};
