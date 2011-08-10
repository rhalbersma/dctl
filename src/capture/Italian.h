#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace rules { struct Italian; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<rules::Italian>
{
public:
        // constructors
        Value();

        // predicates
        bool is_large(BitBoard) const;
        bool operator< (const Value<rules::Italian>&) const;
        bool operator==(const Value<rules::Italian>&) const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
        void toggle_with_king();
        void toggle_promotion() {};

private:
        // implementation
        static const bool TOGGLE = true;

        // representation
        BitBoard piece_order_;
        PieceCount num_pieces_;
        PieceCount num_kings_;
        bool with_king_;
};
        
}       // namespace capture
}       // namespace dctl
