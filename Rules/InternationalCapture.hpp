struct InternationalRules;

template<>
struct init<InternationalRules>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = 0;
        }
};

template<>
struct copy<InternationalRules>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
        }
};

template<>
struct increment<InternationalRules>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                ++capture.num_pieces;
        }
};

template<>
struct decrement<InternationalRules>
{
        void operator()(CaptureValue& capture, BitBoard, BitBoard) const
        {
                --capture.num_pieces;
        }
};

template<>
struct equal_to<InternationalRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces == right.num_pieces;
        }
};

template<>
struct greater_equal<InternationalRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return left.num_pieces >= right.num_pieces;
        }
};
