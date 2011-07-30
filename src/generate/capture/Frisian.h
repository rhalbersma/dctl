#pragma once
#include "Value.h"

namespace dctl {

namespace rules { struct Frisian; };

namespace generate {
namespace capture {

template<>
struct Value<rules::Frisian>
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
        bool with_king;
};
        
template<> bool operator<(const Value<rules::Frisian>&, const Value<rules::Frisian>&);
template<> bool operator==(const Value<rules::Frisian>&, const Value<rules::Frisian>&);

}       // namespace capture
}       // namespace generate
}       // namespace dctl
