#pragma once
#include "Value.h"

namespace dctl {

namespace rules { struct Russian; };

namespace generate {
namespace capture {

template<>
struct Value<rules::Russian>
{
        // constructors
        Value();

        // views
        bool is_large(BitBoard) const;

        // modifiers
        void increment(BitBoard, BitBoard) {};
        void decrement(BitBoard, BitBoard) {};
        
        // representation
        bool promotion;
};
        
}       // namespace capture
}       // namespace generate
}       // namespace dctl
