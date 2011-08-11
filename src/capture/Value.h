#pragma once
#include "Variants.h"
#include "ValueInterface.h"
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace capture {

template<typename Rules>
class Value: public ValueInterface<Rules> {};

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
