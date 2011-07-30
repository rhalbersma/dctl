#pragma once
#include "../../utils/IntegerTypes.h"

namespace dctl {
namespace generate {
namespace capture {

template<typename Rules>
struct Value
{
        // views
        bool is_large(BitBoard) const;

        // modifiers
        void increment(BitBoard, BitBoard) {};
        void decrement(BitBoard, BitBoard) {};
};

template<typename Rules> bool operator<(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator>(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator>=(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator<=(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator==(const Value<Rules>&, const Value<Rules>&);
template<typename Rules> bool operator!=(const Value<Rules>&, const Value<Rules>&);

}       // namespace capture
}       // namespace generate
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Value.hpp"
