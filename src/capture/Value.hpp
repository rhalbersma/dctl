#include "../bit/Bit.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

template<typename Rules>
bool Value<Rules>::do_is_large(BitBoard captured_pieces) const 
{ 
        return bit::count(captured_pieces) >= rules::large_capture<Rules>::value; 
}

template<typename Rules>
bool operator<(const Value<Rules>&, const Value<Rules>&)
{
        // MUST be overriden by derived classes that have capture precedence semantics
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
