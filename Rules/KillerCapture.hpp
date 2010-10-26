struct KillerRules;

template<>
struct init<KillerRules>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = 0;
        }
};

template<>
struct copy<KillerRules>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<KillerRules>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                ++capture.num_pieces;
        }
};

template<>
struct decrement<KillerRules>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                --capture.num_pieces;
        }
};

template<>
struct equal_to<KillerRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<KillerRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};
