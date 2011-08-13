#pragma once
#include "ValueInterface.h"

namespace dctl {
namespace capture {

template<typename Rules>
class Value
: 
        public ValueInterface 
{
private:
        // implementation
        virtual bool do_is_large(BitBoard) const;
};

// these relational operator templates should not be defined on ValueInterface 
// because non-template functions take precedence during overload resolution
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
