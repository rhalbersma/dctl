namespace Variant {

struct Spanish;

template<>
struct init<Spanish>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = capture.num_kings = 0;
        }
};

template<>
struct copy<Spanish>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
        }
};

template<>
struct increment<Spanish>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                ++capture.num_pieces;
                if (target_sq & opponent_kings)
                        ++capture.num_kings;
        }
};

template<>
struct decrement<Spanish>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                if (target_sq & opponent_kings)
                        --capture.num_kings;
                --capture.num_pieces;
        }
};

template<>
struct equal_to<Spanish>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return (
                        (left.num_pieces == right.num_pieces) &&
                         (left.num_kings == right.num_kings)
                );
        }
};

template<>
struct greater_equal<Spanish>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                if (left.num_pieces > right.num_pieces)
                        return true;
                if (left.num_pieces < right.num_pieces)
                        return false;

                return left.num_kings >= right.num_kings;
        }
};

}       // namespace Variant
