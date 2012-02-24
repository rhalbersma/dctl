#pragma once
#include "Value_fwd.hpp"

namespace dctl {
namespace capture {

// primary template
template<typename Rules> 
struct Value 
{
        /* empty */
};

template<typename Rules>
bool operator<(const Value<Rules>& /* left */, const Value<Rules>& /* right */)
{
        return false;
}

template<typename Rules>
bool operator>(const Value<Rules>& left, const Value<Rules>& right)
{        
        return right < left;                            // false by default
}

template<typename Rules>
bool operator>=(const Value<Rules>& left, const Value<Rules>& right)
{        
        return !(left < right);                         // true by default
}

template<typename Rules>
bool operator<=(const Value<Rules>& left, const Value<Rules>& right)
{        
        return !(right < left);                         // true by default
}

template<typename Rules>
bool operator==(const Value<Rules>& left, const Value<Rules>& right)
{        
        return !(left < right) && !(right < left);      // true by default
}

template<typename Rules>
bool operator!=(const Value<Rules>& left, const Value<Rules>& right)
{        
        return !(left == right);                        // false by default
}

template<typename Rules>
bool is_promotion(const Value<Rules>& /* v */)
{
        return false;
}

template<typename Rules>
void increment(Value<Rules>& /* v */)
{
        /* no-op */
}

template<typename Rules>
void increment(Value<Rules>& /* v */, BitBoard /* target_sq */, BitBoard /* king_targets */)
{
        /* no-op */
}

template<typename Rules>
void decrement(Value<Rules>& /* v */)
{
        /* no-op */
}

template<typename Rules>
void decrement(Value<Rules>& /* v */, BitBoard /* target_sq */, BitBoard /* king_targets */)
{
        /* no-op */   
}

template<typename Rules>
void toggle_with_king(Value<Rules>& /* v */)
{
        /* no-op */
}

template<typename Rules>
void toggle_promotion(Value<Rules>& /* v */)
{
        /* no-op */   
}

}       // namespace capture
}       // namespace dctl
