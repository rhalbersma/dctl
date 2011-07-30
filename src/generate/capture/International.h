#pragma once
#include "Value.h"

namespace dctl {

namespace rules { struct International; };

namespace generate {
namespace capture {

template<>
struct Value<rules::International>
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
        
template<> bool operator<(const Value<rules::International>&, const Value<rules::International>&);
template<> bool operator==(const Value<rules::International>&, const Value<rules::International>&);

}       // namespace capture
}       // namespace generate
}       // namespace dctl
