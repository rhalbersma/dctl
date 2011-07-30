#pragma once
#include "Variants.h"
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace capture {

template<typename Rules>
class Value
{
public:
        // predicates
        bool is_large(BitBoard) const;

        // modifiers
        void increment(BitBoard, BitBoard) {};
        void decrement(BitBoard, BitBoard) {};
        void toggle_with_king() {};
        void toggle_promotion() {};
};

template<typename Rules> bool operator< (const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator> (const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator>=(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator<=(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator==(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator!=(const Value<Rules>&, const Value<Rules>&);

}       // namespace capture
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Value.hpp"
