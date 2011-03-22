namespace Variants { struct Italian; }

namespace Tree {
namespace Generate {
namespace Capture {

template<>
struct init<Variants::Italian>
{
        void operator()(Value& value) const
        {
                value.piece_order = 0;
                value.num_pieces = value.num_kings = 0;
                value.with_king = false;
        }
};

template<>
struct copy<Variants::Italian>
{
        void operator()(Value& left, const Value& right) const
        {
                left.piece_order = right.piece_order;
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
                left.with_king = right.with_king;
        }
};

template<>
struct increment<Variants::Italian>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                ++value.num_pieces;
                if (target_sq & king_targets) {
                        ++value.num_kings;
                        value.piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - value.num_pieces);
                }
        }
};

template<>
struct decrement<Variants::Italian>
{
        void operator()(Value& value, BitBoard target_sq, BitBoard king_targets) const
        {
                if (target_sq & king_targets) {
                        value.piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - value.num_pieces);
                        --value.num_kings;
                }
                --value.num_pieces;
        }
};

template<>
struct equal_to<Variants::Italian>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                return (
                       (left.piece_order == right.piece_order) &&
                        (left.num_pieces == right.num_pieces) &&
                         (left.num_kings == right.num_kings) &&
                         (left.with_king == right.with_king)
                );
        }
};

template<>
struct greater_equal<Variants::Italian>: public std::binary_function<Value, Value, bool>
{
        bool operator()(const Value& left, const Value& right) const
        {
                if (left.num_pieces > right.num_pieces)
                        return true;
                if (left.num_pieces < right.num_pieces)
                        return false;

                if (left.with_king > right.with_king)
                        return true;
                if (left.with_king < right.with_king)
                        return false;

                if (left.num_kings > right.num_kings)
                        return true;
                if (left.num_kings < right.num_kings)
                        return false;

                return left.piece_order >= right.piece_order;
        }
};

}       // namespace Capture
}       // namespace Generate
}       // namespace Tree
