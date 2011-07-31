#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

class Material
{
public:
        // typedefs 
        enum { NONE, PAWN, KING, BOTH };

        // constructors
        Material() {};                                  // no default initialization
        explicit Material(BitBoard);                    // zero initialize
        Material(BitBoard, BitBoard, BitBoard);         // initialize with a set of bitboards

        // post-conditions for constructors and modifiers
        bool invariant() const;                         // logical consistency of the representation

        // views
        BitBoard men(bool) const;                       // black or white men
        BitBoard kings(bool) const;                     // black or white kings
        BitBoard pieces(bool) const;                    // black or white pieces
        BitBoard men() const;                           // black and white men
        BitBoard kings() const;                         // black and white kings
        BitBoard pieces() const;                        // black and white pieces

        // modifiers
        template<bool> 
        void init(BitBoard, BitBoard, BitBoard);        // initialize with a set of bitboards

        template<bool>
        static Material create(BitBoard, BitBoard, BitBoard);

        Material& operator^=(const Material&);          // xor-assign the set bits of another piece set

private:
        // representation
        BitBoard pieces_[2];                            // black and white pieces
        BitBoard kings_;                                // kings
};

const Material operator^(const Material&, const Material&);     // xor-assign the set bits of two piece sets

// predicates
bool operator==(const Material&, const Material&);      // equality operator
bool operator!=(const Material&, const Material&);      // inequality operator

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Material.hpp"
