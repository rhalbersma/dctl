#pragma once
#include <vector>
#include "Material.h"
#include "Side.h"
#include "../utils/IntegerTypes.h"
#include "../utils/Ply.h"

namespace dctl {

class Move
{
public:
        // typedefs
        enum { JUMPS, MOVES };

        // constructors
        Move() {};
        Move(const Material&);

        // create a new object from a set of bitboards
        template<bool> 
        static Move create(BitBoard, BitBoard, BitBoard);

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

private:
        Material material_;
        static const bool to_move_ = Side::PASS;
};

// predicates
bool operator==(const Move&, const Move&);              // equality operator
bool operator!=(const Move&, const Move&);              // inequality operator

// typedefs
typedef std::vector<int> Sequence;
typedef std::vector<int> Order;

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Move.hpp"
