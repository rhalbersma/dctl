namespace Variant {

struct Italian;

template<>
struct init<Italian>
{
        void operator()(CaptureValue& capture) const
        {
                capture.piece_order = 0;
                capture.num_pieces = capture.num_kings = 0;
                capture.with_king = false;
        }
};

template<>
struct copy<Italian>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.piece_order = right.piece_order;
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
                left.with_king = right.with_king;
        }
};

template<>
struct increment<Italian>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard king_targets) const
        {
                ++capture.num_pieces;
                if (target_sq & king_targets) {
                        ++capture.num_kings;
                        capture.piece_order ^= BitBoard(1) << (BITBOARD_CAPACITY - capture.num_pieces);
                }
        }
};

template<>
struct decrement<Italian>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard king_targets) const
        {
                if (target_sq & king_targets) {
                        capture.piece_order ^= BitBoard(1) << (BITBOARD_CAPACITY - capture.num_pieces);
                        --capture.num_kings;
                }
                --capture.num_pieces;
        }
};

template<>
struct equal_to<Italian>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
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
struct greater_equal<Italian>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
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

}       // namespace Variant
