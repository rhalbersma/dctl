#pragma once
#include "Value.h"

namespace dctl {

namespace rules { struct Spanish; };

namespace generate {
namespace capture {

template<>
struct Value<rules::Spanish>
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
        PieceCount num_kings;
};
        
template<> bool operator<(const Value<rules::Spanish>&, const Value<rules::Spanish>&);
template<> bool operator==(const Value<rules::Spanish>&, const Value<rules::Spanish>&);

}       // namespace capture
}       // namespace generate
}       // namespace dctl
