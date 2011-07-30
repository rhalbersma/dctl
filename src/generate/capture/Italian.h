#pragma once
#include "Value.h"

namespace dctl {

namespace rules { struct Italian; };

namespace generate {
namespace capture {

template<>
struct Value<rules::Italian>
{
        // constructors
        Value();

        // views
        bool is_large(BitBoard) const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
               
        // representation
        BitBoard piece_order;
        PieceCount num_pieces;
        PieceCount num_kings;
        bool with_king;
};
        
template<> bool operator<(const Value<rules::Italian>&, const Value<rules::Italian>&);
template<> bool operator==(const Value<rules::Italian>&, const Value<rules::Italian>&);

}       // namespace capture
}       // namespace generate
}       // namespace dctl
