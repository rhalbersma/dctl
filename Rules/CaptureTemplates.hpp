#include "CaptureValue.h"
#include "Rules.h"
#include "../Utilities/Bit.h"

// tag dispatching on quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const CaptureValue& capture, BitBoard captured_pieces) const
{
        return large<Rules>()(capture, captured_pieces, Int2Type<is_MajorityPrecedence<Rules>::VALUE>());
}

// partial specialization for no quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const CaptureValue&, BitBoard captured_pieces, Int2Type<false>) const
{
        return Bit::count(captured_pieces) >= LargeCaptureValue<Rules>::VALUE;
}

// partial specialization for quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const CaptureValue& capture, BitBoard, Int2Type<true>) const
{
        return capture.num_pieces >= LargeCaptureValue<Rules>::VALUE;
}

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "CaptureSpecializations.hpp"
