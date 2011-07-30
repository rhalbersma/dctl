#include "../rules/Rules.h"
#include "../utils/Bit.h"

namespace dctl {
namespace capture {

template<typename Rules>
bool Value<Rules>::is_large(BitBoard captured_pieces) const
{
        return bit::count(captured_pieces) >= rules::large_capture<Rules>::value;
}

template<typename Rules>
bool operator<(const Value<Rules>&, const Value<Rules>&)
{
        return false;
}

template<typename Rules>
bool operator>(const Value<Rules>& left, const Value<Rules>& right)
{
        return right < left;
}

template<typename Rules>
bool operator>=(const Value<Rules>& left, const Value<Rules>& right)
{
        return !(left < right);
}

template<typename Rules>
bool operator<=(const Value<Rules>& left, const Value<Rules>& right)
{
        return !(right < left);
}

template<typename Rules>
bool operator==(const Value<Rules>& left, const Value<Rules>& right)
{
        return !(left < right) && !(right < left);
}

template<typename Rules>
bool operator!=(const Value<Rules>& left, const Value<Rules>& right)
{
        return !(left == right);
}

}       // namespace capture
}       // namespace dctl
