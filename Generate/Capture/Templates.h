#pragma once
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include <functional>

namespace Capture {

struct Value;

template<typename> struct init      { void operator()(Value&)                      const {} };
template<typename> struct copy      { void operator()(Value&, const Value&)        const {} };
template<typename> struct increment { void operator()(Value&, BitBoard, BitBoard)  const {} };
template<typename> struct decrement { void operator()(Value&, BitBoard, BitBoard)  const {} };

template<typename>
class large: public std::binary_function<Value, BitBoard, bool>
{
public:
        bool operator()(const Value&, BitBoard) const;

private:
        bool operator()(const Value&, BitBoard, Int2Type<false>) const;
        bool operator()(const Value&, BitBoard, Int2Type<true>) const;
};

template<typename> 
struct equal_to: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value&, const Value&) const
        {
                return true;
        }
};

template<typename Rules>
struct not_equal_to: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return !equal_to<Rules>()(left, right);
        }
};

template<typename>
struct greater_equal: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value&, const Value&) const
        {
                return true;
        }
};

}       // namespace Capture

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Templates.hpp"
