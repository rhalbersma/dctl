#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace rules { struct Russian; };

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<rules::Russian>
{
public:
        // constructors
        Value();

        // predicates
        bool is_large(BitBoard) const;
        bool is_promotion() const;

        // modifiers
        void increment(BitBoard, BitBoard) {};
        void decrement(BitBoard, BitBoard) {};
        void toggle_with_king() {};
        void toggle_promotion();
        
private:
        // implementation
        static const bool TOGGLE = true;

        // representation
        bool promotion_;
};
        
}       // namespace capture
}       // namespace dctl
