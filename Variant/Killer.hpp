namespace Variant {

struct Killer;

template<>
struct init<Killer>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = 0;
        }
};

template<>
struct copy<Killer>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<Killer>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                ++capture.num_pieces;
        }
};

template<>
struct decrement<Killer>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                --capture.num_pieces;
        }
};

template<>
struct equal_to<Killer>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<Killer>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};

}       // namespace Variant
