namespace Variants { struct Frisian; }

namespace Tree {
namespace Generate {
namespace Capture {

template<>
struct init<Variants::Frisian>
{
        void operator()(Value& value) const
        {
                value.num_pieces = value.num_kings = 0;
                value.with_king = false;
        }
};

template<>
struct copy<Variants::Frisian>
{
        void operator()(Value& left, const Value& right) const
        {
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
                left.with_king = right.with_king;
        }
};

template<>
struct increment<Variants::Frisian>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                ++value.num_pieces;
                if (target_sq & king_targets)
                        ++value.num_kings;
        }
};

template<>
struct decrement<Variants::Frisian>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                if (target_sq & king_targets)
                        --value.num_kings;
                --value.num_pieces;
        }
};

template<>
struct equal_to<Variants::Frisian>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return (
                        (left.num_pieces == right.num_pieces) &&
                         (left.num_kings == right.num_kings) &&
                         (left.with_king == right.with_king)
                );
        }
};

template<>
struct greater_equal<Variants::Frisian>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                int delta_kings = left.num_kings - right.num_kings;
                int delta_men = left.num_pieces - right.num_pieces - delta_kings;

                if (delta_kings * delta_men < 0) {
                        if (delta_kings > 0)
                                return delta_men + 2 * delta_kings - 1 >= 0;
                        else
                                return delta_men + 2 * delta_kings     >= 0;
                } else {
                        if (delta_men + delta_kings > 0)
                                return true;
                        if (delta_men + delta_kings < 0)
                                return false;

                        return left.with_king >= right.with_king;
                }
        }
};

}       // namespace Capture
}       // namespace Generate
}       // namespace Tree
