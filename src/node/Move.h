#pragma once
#include "Material.h"
#include "Side.h"
#include "../utils/IntegerTypes.h"
#include "../utils/Ply.h"
#include "../utils/VectorArray.h"

namespace dctl {

class Move
{
public:
        // typedefs
        enum { JUMPS, MOVES };

        // views
        const Material& material() const;
        bool to_move() const;

        BitBoard men(bool) const;                       // black or white men
        BitBoard kings(bool) const;                     // black or white kings
        BitBoard pieces(bool) const;                    // black or white pieces
        BitBoard men() const;                           // black and white men
        BitBoard kings() const;                         // black and white kings
        BitBoard pieces() const;                        // black and white pieces

        // post-conditions for constructors and modifiers
        bool invariant() const;                         // logical consistency of the representation

        // modifiers
        template<bool> 
        void init(BitBoard, BitBoard, BitBoard);        // initialize with a set of bitboards

private:
        Material material_;
        static const bool to_move_ = Side::PASS;
};

// predicates
bool operator==(const Move&, const Move&);              // equality operator
bool operator!=(const Move&, const Move&);              // inequality operator

// typedefs
typedef VectorArray<int, MAX_PLY> Sequence;
typedef VectorArray<int, MAX_MOVES> Order;
typedef VectorArray<Move, MAX_MOVES> Stack;

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Move.hpp"
