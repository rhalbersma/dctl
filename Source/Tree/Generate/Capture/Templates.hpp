#include "Value.h"
#include "../../../Variants/Rules.h"
#include "../../../Utilities/Bit.h"

namespace Tree {
namespace Generate {
namespace Capture {

// tag dispatching on quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value& value, BitBoard captured_pieces) const
{
        return large<Rules>()(value, captured_pieces, Int2Type<Variants::is_majority_precedence<Rules>::value>());
}

// partial specialization for no quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value&, BitBoard captured_pieces, Int2Type<false>) const
{
        return Bit::count(captured_pieces) >= Variants::large_capture<Rules>::value;
}

// partial specialization for quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value& value, BitBoard, Int2Type<true>) const
{
        return value.num_pieces >= Variants::large_capture<Rules>::value;
}

}       // namespace Capture
}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Specializations.hpp"
