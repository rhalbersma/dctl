#include "Templates.h"
#include "Value.h"

namespace dctl {
namespace generate {
namespace capture {

template<>
struct init<rules::International>
{
        void operator()(Value& value) const
        {
                value.num_pieces = 0;
        }
};

template<>
struct copy<rules::International>
{
        void operator()(Value& left, const Value& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<rules::International>
{
        void operator()(Value& value, BitBoard, BitBoard) const
        {
                ++value.num_pieces;
        }
};

template<>
struct decrement<rules::International>
{
        void operator()(Value& value, BitBoard, BitBoard) const
        {
                --value.num_pieces;
        }
};

template<>
struct equal_to<rules::International>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<rules::International>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};

}       // namespace capture
}       // namespace generate
}       // namespace dctl
