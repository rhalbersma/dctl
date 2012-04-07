#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"

namespace dctl {
namespace capture {

// primary template
template<typename Rules>
struct Value
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<Rules> >
{
        // predicates
        bool operator<(const Value<Rules>& /* other */) const
        {
                return false;
        }

        bool operator==(const Value<Rules>& /* other */) const
        {

                return true;
        }
};

template<typename Rules>
bool is_large(const Value<Rules>& v, BitBoard captured_pieces)
{
        return count(v, captured_pieces) >= rules::large_capture<Rules>::value;
}

template<typename Rules>
int count(const Value<Rules>& v, BitBoard captured_pieces)
{
        return aux::count(
                v, captured_pieces,
                Int2Type<rules::is_majority_precedence<Rules>::value>()
        );
}

template<typename Rules>
void increment(Value<Rules>& v, bool is_captured_king)
{
        aux::increment(
                v, is_captured_king,
                Int2Type<rules::precedence_type<Rules>::value>()
        );
}

template<typename Rules>
void decrement(Value<Rules>& v, bool is_captured_king)
{
        aux::decrement(
                v, is_captured_king,
                Int2Type<rules::precedence_type<Rules>::value>()
        );
}

template<typename Rules>
bool is_promotion(const Value<Rules>& v)
{
        return aux::is_promotion(
                v,
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

template<typename Rules>
void toggle_promotion(Value<Rules>& v)
{
        aux::toggle_promotion(
                v,
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

template<typename Rules>
void toggle_with_king(Value<Rules>& v)
{
        aux::toggle_with_king(
                v,
                Int2Type<rules::is_relative_king_precedence<Rules>::value>()
        );
}

namespace aux {

template<typename Rules>
int count(const Value<Rules>& /* v */, BitBoard captured_pieces, Int2Type<false>)
{
        return bit::count(captured_pieces);
}

template<typename Rules>
int count(const Value<Rules>& v, BitBoard /* captured_pieces */, Int2Type<true>)
{
        return v.count();
}

template<typename Rules>
void increment(Value<Rules>& /* v */, bool /* is_captured_king */, Int2Type<rules::precede_0>)
{
        // no-op
}

template<typename Rules>
void increment(Value<Rules>& v, bool /* is_captured_king */, Int2Type<rules::precede_1>)
{
        v.increment();
}

template<typename Rules>
void increment(Value<Rules>& v, bool is_captured_king, Int2Type<rules::precede_Q>)
{
        v.increment(is_captured_king);
}

template<typename Rules>
void decrement(Value<Rules>& /* v */, bool /* is_captured_king */, Int2Type<rules::precede_0>)
{
        // no-op
}

template<typename Rules>
void decrement(Value<Rules>& v, bool /* is_captured_king */, Int2Type<rules::precede_1>)
{
        v.decrement();
}

template<typename Rules>
void decrement(Value<Rules>& v, bool is_captured_king, Int2Type<rules::precede_Q>)
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
        // no-op
}

template<typename Rules>
void toggle_promotion(Value<Rules>& v, Int2Type<rules::promote_ep>)
{
        v.toggle_promotion();
}

template<typename Rules>
void toggle_with_king(Value<Rules>& /* v */, Int2Type<false>)
{
        // no-op
}

template<typename Rules>
void toggle_with_king(Value<Rules>& v, Int2Type<true>)
{
        v.toggle_with_king();
}

}       // namespace aux
}       // namespace capture
}       // namespace dctl
