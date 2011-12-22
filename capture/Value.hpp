#pragma once
#include "ValueInterface.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"

namespace dctl {
namespace capture {

template<typename Rules>
class Value
: 
        public ValueInterface 
{
private:
        // implementation
        virtual bool do_is_large(BitBoard captured_pieces) const 
        { 
                return bit::count(captured_pieces) >= rules::large_capture<Rules>::value; 
        }
};

template<typename Rules>
bool operator<(const Value<Rules>& /* left */, const Value<Rules>& /* right */)
{
        // MUST be overriden by template specializations for Rules instances 
        // that have capture precedence semantics
        return false;
}

template<typename Rules>
bool operator>(const Value<Rules>& left, const Value<Rules>& right)
{
        // false by default
        return right < left;
}

template<typename Rules>
bool operator>=(const Value<Rules>& left, const Value<Rules>& right)
{
        // true by default
        return !(left < right);
}

template<typename Rules>
bool operator<=(const Value<Rules>& left, const Value<Rules>& right)
{
        // true by default
        return !(right < left);
}

template<typename Rules>
bool operator==(const Value<Rules>& left, const Value<Rules>& right)
{
        // true by default
        return !(left < right) && !(right < left);
}

template<typename Rules>
bool operator!=(const Value<Rules>& left, const Value<Rules>& right)
{
        // false by default
        return !(left == right);
}

}       // namespace capture
}       // namespace dctl
