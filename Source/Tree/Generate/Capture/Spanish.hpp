namespace variants { struct Spanish; }

namespace tree {
namespace generate {
namespace capture {

template<>
struct init<variants::Spanish>
{
        void operator()(Value& value) const
        {
                value.num_pieces = value.num_kings = 0;
        }
};

template<>
struct copy<variants::Spanish>
{
        void operator()(Value& left, const Value& right) const
        {
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
        }
};

template<>
struct increment<variants::Spanish>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                ++value.num_pieces;
                if (target_sq & king_targets)
                        ++value.num_kings;
        }
};

template<>
struct decrement<variants::Spanish>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                if (target_sq & king_targets)
                        --value.num_kings;
                --value.num_pieces;
        }
};

template<>
struct equal_to<variants::Spanish>: public std::binary_function<Value, Value, bool>
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
struct greater_equal<variants::Spanish>: public std::binary_function<Value, Value, bool>
{
        // http://fpdamas.home.sapo.pt/regrasclass.htm
        bool operator()(const Value& left, const Value& right) const
        {
                // 15 b)
                if (left.num_pieces > right.num_pieces)
                        return true;
                if (left.num_pieces < right.num_pieces)
                        return false;

                // 15 c)
                return left.num_kings >= right.num_kings;
        }
};

}       // namespace capture
}       // namespace generate
}       // namespace tree
