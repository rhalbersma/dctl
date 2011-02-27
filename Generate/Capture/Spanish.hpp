namespace Variant { struct Spanish; }

namespace Capture {

template<>
struct init<Variant::Spanish>
{
        void operator()(Value& value) const
        {
                value.num_pieces = value.num_kings = 0;
        }
};

template<>
struct copy<Variant::Spanish>
{
        void operator()(Value& left, const Value& right) const
        {
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
        }
};

template<>
struct increment<Variant::Spanish>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                ++value.num_pieces;
                if (target_sq & king_targets)
                        ++value.num_kings;
        }
};

template<>
struct decrement<Variant::Spanish>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                if (target_sq & king_targets)
                        --value.num_kings;
                --value.num_pieces;
        }
};

template<>
struct equal_to<Variant::Spanish>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return (
                        (left.num_pieces == right.num_pieces) &&
                         (left.num_kings == right.num_kings)
                );
        }
};

template<>
struct greater_equal<Variant::Spanish>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                if (left.num_pieces > right.num_pieces)
                        return true;
                if (left.num_pieces < right.num_pieces)
                        return false;

                return left.num_kings >= right.num_kings;
        }
};

}       // namespace Capture
