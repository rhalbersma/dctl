struct SpanishRules;

template<>
struct init<SpanishRules>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = capture.num_kings = 0;
        }
};

template<>
struct copy<SpanishRules>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
        }
};

template<>
struct increment<SpanishRules>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                ++capture.num_pieces;
                if (target_sq & opponent_kings)
                        ++capture.num_kings;
        }
};

template<>
struct decrement<SpanishRules>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                if (target_sq & opponent_kings)
                        --capture.num_kings;
                --capture.num_pieces;
        }
};

template<>
struct equal_to<SpanishRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
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
struct greater_equal<SpanishRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
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
