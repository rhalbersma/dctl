struct ItalianRules;

template<>
struct init<ItalianRules>
{
        void operator()(CaptureValue& capture) const
        {
                capture.piece_order = 0;
                capture.num_pieces = capture.num_kings = 0;
                capture.with_king = false;
        }
};

template<>
struct copy<ItalianRules>
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
struct increment<ItalianRules>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                ++capture.num_pieces;
                if (target_sq & opponent_kings) {
                        ++capture.num_kings;
                        capture.piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - capture.num_pieces);
                }
        }
};

template<>
struct decrement<ItalianRules>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                if (target_sq & opponent_kings) {
                        capture.piece_order ^= BitBoard(1) << (8 * sizeof(BitBoard) - capture.num_pieces);
                        --capture.num_kings;
                }
                --capture.num_pieces;
        }
};

template<>
struct equal_to<ItalianRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
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
struct greater_equal<ItalianRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
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
