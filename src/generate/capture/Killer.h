#pragma once
#include "Value.h"

namespace dctl {

namespace rules { struct Killer; };

namespace generate {
namespace capture {

template<>
struct Value<rules::Killer>
{
        // constructors
        Value();

        // views
        bool is_large(BitBoard) const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
        
        // representation
        PieceCount num_pieces;
};
        
template<> bool operator<(const Value<rules::Killer>&, const Value<rules::Killer>&);
template<> bool operator==(const Value<rules::Killer>&, const Value<rules::Killer>&);

}       // namespace capture
}       // namespace generate
}       // namespace dctl
