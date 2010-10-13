#pragma once
#include "../../Utilities/IntegerTypes.h"

class Pieces
{
public:
        // typedefs 
        enum Composition {NONE, ONLY_MEN, ONLY_KINGS, MEN_AND_KINGS};

        // constructors
        Pieces(void) {};
        explicit Pieces(BitBoard);                      // zero initialize
        Pieces(BitBoard, BitBoard, BitBoard);           // initialize with a set of bitboards

        // predicates
        bool operator==(const Pieces&) const;
        bool operator!=(const Pieces&) const;

        // views
        BitBoard men(bool) const;                       // black or white men
        BitBoard kings(bool) const;                     // black or white kings
        BitBoard pieces(bool) const;                    // black or white pieces
        BitBoard men(void) const;                       // black and white men
        BitBoard kings(void) const;                     // black and white kings
        BitBoard occupied(void) const;                  // black and white pieces
        Composition composition(bool) const;            // composition of black or white pieces

        // modifiers
        template<bool> 
        void init(BitBoard, BitBoard, BitBoard);        // initialize with a set of bitboards

        Pieces& toggle(const Pieces&);                  // toggle the set bits of another piece set

private:
        // post-conditions for constructors and modifiers
        bool invariant(void) const;                    // logical consistency of the representation

        // representation
        BitBoard d_pieces[2];                           // black and white pieces
        BitBoard d_kings;                               // kings
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Pieces.hpp"
