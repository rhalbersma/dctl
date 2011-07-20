#include "Value.h"
#include "../../../rules/Traits.h"
#include "../../../utils/Bit.h"

namespace tree {
namespace generate {
namespace capture {

// tag dispatching on quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value& value, BitBoard captured_pieces) const
{
        return large<Rules>()(value, captured_pieces, Int2Type<rules::is_majority_precedence<Rules>::value>());
}

// partial specialization for no quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value&, BitBoard captured_pieces, Int2Type<false>) const
{
        return bit::count(captured_pieces) >= rules::large_capture<Rules>::value;
}

// partial specialization for quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value& value, BitBoard, Int2Type<true>) const
{
        return value.num_pieces >= rules::large_capture<Rules>::value;
}

}       // namespace capture
}       // namespace generate
}       // namespace tree
