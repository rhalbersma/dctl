namespace Variant { struct International; }

namespace Capture {

template<>
struct init<Variant::International>
{
        void operator()(Value& value) const
        {
                value.num_pieces = 0;
        }
};

template<>
struct copy<Variant::International>
{
        void operator()(Value& left, const Value& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<Variant::International>
{
        void operator()(Value& value, BitBoard, BitBoard) const
        {
                ++value.num_pieces;
        }
};

template<>
struct decrement<Variant::International>
{
        void operator()(Value& value, BitBoard, BitBoard) const
        {
                --value.num_pieces;
        }
};

template<>
struct equal_to<Variant::International>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<Variant::International>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};

}       // namespace Capture
