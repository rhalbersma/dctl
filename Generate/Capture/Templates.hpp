#include "Value.h"
#include "../../Variant/Rules.h"
#include "../../Utilities/Bit.h"

namespace Capture {

// tag dispatching on quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value& value, BitBoard captured_pieces) const
{
        return large<Rules>()(value, captured_pieces, Int2Type<Variant::is_MajorityPrecedence<Rules>::VALUE>());
}

// partial specialization for no quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value&, BitBoard captured_pieces, Int2Type<false>) const
{
        return Bit::count(captured_pieces) >= Variant::LargeCaptureValue<Rules>::VALUE;
}

// partial specialization for quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const Value& value, BitBoard, Int2Type<true>) const
{
        return value.num_pieces >= Variant::LargeCaptureValue<Rules>::VALUE;
}

}       // namespace Capture

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Specializations.hpp"
