namespace Variants { struct International; }

namespace Tree {
namespace Generate {
namespace Capture {

template<>
struct init<Variants::International>
{
        void operator()(Value& value) const
        {
                value.num_pieces = 0;
        }
};

template<>
struct copy<Variants::International>
{
        void operator()(Value& left, const Value& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<Variants::International>
{
        void operator()(Value& value, BitBoard, BitBoard) const
        {
                ++value.num_pieces;
        }
};

template<>
struct decrement<Variants::International>
{
        void operator()(Value& value, BitBoard, BitBoard) const
        {
                --value.num_pieces;
        }
};

template<>
struct equal_to<Variants::International>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<Variants::International>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};

}       // namespace Capture
}       // namespace Generate
}       // namespace Tree
