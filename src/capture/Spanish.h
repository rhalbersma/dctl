#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace rules { struct Spanish; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<rules::Spanish>
{
public:
        // constructors
        Value();

        // predicates
        bool is_large(BitBoard) const;
        bool operator< (const Value<rules::Spanish>&) const;
        bool operator==(const Value<rules::Spanish>&) const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
        void toggle_with_king() {};
        void toggle_promotion() {};
           
private:
        // representation
        PieceCount num_pieces_;
        PieceCount num_kings_;
};
        
}       // namespace capture
}       // namespace dctl
