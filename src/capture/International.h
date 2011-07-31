#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace rules { struct International; };

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<rules::International>
{
public:
        // constructors
        Value();

        // predicates
        bool is_large(BitBoard) const;
        bool operator< (const Value<rules::International>&) const;
        bool operator==(const Value<rules::International>&) const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
        void toggle_with_king() {};
        void toggle_promotion() {};

private:
        // representation
        PieceCount num_pieces_;
};
        
}       // namespace capture
}       // namespace dctl
