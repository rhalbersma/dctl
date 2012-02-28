#pragma once
#include "Value_fwd.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"

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
void increment(Value<Rules>& v, bool is_captured_king)
{
        dispatch::increment(
                v, is_captured_king,
                Int2Type<rules::precedence_type<Rules>::value>()
        );
}

template<typename Rules>
void decrement(Value<Rules>& v, bool is_captured_king)
{
        dispatch::decrement(
                v, is_captured_king,
                Int2Type<rules::precedence_type<Rules>::value>()
        );
}

template<typename Rules>
bool is_promotion(const Value<Rules>& v)
{
        return dispatch::is_promotion(
                v, 
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

template<typename Rules>
void toggle_promotion(Value<Rules>& v)
{
        dispatch::toggle_promotion(
                v,
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

template<typename Rules>
void toggle_with_king(Value<Rules>& v)
{
        dispatch::toggle_with_king(
                v,
                Int2Type<rules::is_relative_king_precedence<Rules>::value>()
        );
}

namespace dispatch {

template<typename Rules>
void increment(
        Value<Rules>& /* v */, bool /* is_captured_king */, Int2Type<rules::precede_0>
)
{
        /* no-op */
}

template<typename Rules>
void increment(
        Value<Rules>& v, bool /* is_captured_king */, Int2Type<rules::precede_1>
)
{
        v.increment();
}

template<typename Rules>
void increment(
        Value<Rules>& v, bool is_captured_king, Int2Type<rules::precede_Q>
)
{
        v.increment(is_captured_king);
}

template<typename Rules>
void decrement(
        Value<Rules>& /* v */, bool /* is_captured_king */, Int2Type<rules::precede_0>
)
{
        /* no-op */
}

template<typename Rules>
void decrement(
        Value<Rules>& v, bool /* is_captured_king */, Int2Type<rules::precede_1>
)
{
        v.decrement();
}

template<typename Rules>
void decrement(
        Value<Rules>& v, bool is_captured_king, Int2Type<rules::precede_Q>
)
{
        v.decrement(is_captured_king);
}

template<typename Rules>
bool is_promotion(const Value<Rules>& /* v */, Int2Type<rules::promote_af>)
{
        return false;
}

template<typename Rules>
bool is_promotion(const Value<Rules>& v, Int2Type<rules::promote_ep>)
{
        return v.is_promotion();
}

template<typename Rules>
void toggle_promotion(Value<Rules>& /* v */, Int2Type<rules::promote_af>)
{
        /* no-op */   
}

template<typename Rules>
void toggle_promotion(Value<Rules>& v, Int2Type<rules::promote_ep>)
{
        v.toggle_promotion();
}

template<typename Rules>
void toggle_with_king(Value<Rules>& /* v */, Int2Type<false>)
{
        /* no-op */
}

template<typename Rules>
void toggle_with_king(Value<Rules>& v, Int2Type<true>)
{
        v.toggle_with_king();
}

}       // namespace dispatch
}       // namespace capture
}       // namespace dctl
