#pragma once
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include <functional>

class CaptureValue;

template<typename> struct init      { void operator()(CaptureValue&)                      const {} };
template<typename> struct copy      { void operator()(CaptureValue&, const CaptureValue&) const {} };
template<typename> struct increment { void operator()(CaptureValue&, BitBoard, BitBoard)  const {} };
template<typename> struct decrement { void operator()(CaptureValue&, BitBoard, BitBoard)  const {} };

template<typename>
class large: public std::binary_function<CaptureValue, BitBoard, bool>
{
public:
        bool operator()(const CaptureValue&, BitBoard) const;

private:
        bool operator()(const CaptureValue&, BitBoard, Int2Type<false>) const;
        bool operator()(const CaptureValue&, BitBoard, Int2Type<true>) const;
};

template<typename> 
struct equal_to: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue&, const CaptureValue&) const
        {
                return true;
        }
};

template<typename Rules>
struct not_equal_to: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return !equal_to<Rules>()(left, right);
        }
};

template<typename>
struct greater_equal: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue&, const CaptureValue&) const
        {
                return true;
        }
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "CaptureTemplates.hpp"
