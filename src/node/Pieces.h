#pragma once
#include "../utils/IntegerTypes.h"

namespace dtl {

class Pieces
{
public:
        // typedefs 
        enum { NONE, PAWN, KING, BOTH };

        // constructors
        Pieces() {};                                    // no default initialization
        explicit Pieces(BitBoard);                      // zero initialize
        Pieces(BitBoard, BitBoard, BitBoard);           // initialize with a set of bitboards

        // post-conditions for constructors and modifiers
        bool invariant() const;                         // logical consistency of the representation

        // predicates
        bool operator==(const Pieces&) const;           // equality operator
        bool operator!=(const Pieces&) const;           // inequality operator

        // views
        BitBoard men() const;                           // black and white men
        BitBoard kings() const;                         // black and white kings
        BitBoard occupied() const;                      // black and white pieces
        BitBoard men(bool) const;                       // black or white men
        BitBoard kings(bool) const;                     // black or white kings
        BitBoard pieces(bool) const;                    // black or white pieces

        // modifiers
        template<bool> 
        void init(BitBoard, BitBoard, BitBoard);        // initialize with a set of bitboards

        Pieces& operator^=(const Pieces&);              // xor-assign the set bits of another piece set

private:
        // representation
        BitBoard pieces_[2];                            // black and white pieces
        BitBoard kings_;                                // kings
};

const Pieces operator^(const Pieces&, const Pieces&);   // xor-assign the set bits of two piece sets

}       // namespace dtl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Pieces.hpp"
