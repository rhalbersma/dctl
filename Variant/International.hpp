namespace Variant {

struct International;

template<>
struct init<International>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = 0;
        }
};

template<>
struct copy<International>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<International>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                ++capture.num_pieces;
        }
};

template<>
struct decrement<International>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                --capture.num_pieces;
        }
};

template<>
struct equal_to<International>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<International>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};

}       // namespace Variant
