struct FrisianRules;

template<>
struct init<FrisianRules>
{
        void operator()(CaptureValue& capture) const
        {
                capture.num_pieces = capture.num_kings = 0;
                capture.with_king = false;
        }
};

template<>
struct copy<FrisianRules>
{
        void operator()(CaptureValue& left, const CaptureValue& right) const
        {
                left.num_pieces = right.num_pieces;
                left.num_kings = right.num_kings;
                left.with_king = right.with_king;
        }
};

template<>
struct increment<FrisianRules>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                ++capture.num_pieces;
                if (target_sq & opponent_kings)
                        ++capture.num_kings;
        }
};

template<>
struct decrement<FrisianRules>
{
        void operator()(CaptureValue& capture, BitBoard target_sq, BitBoard opponent_kings) const
        {
                if (target_sq & opponent_kings)
                        --capture.num_kings;
                --capture.num_pieces;
        }
};

template<>
struct equal_to<FrisianRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
        {
                return (
                        (left.num_pieces == right.num_pieces) &&
                         (left.num_kings == right.num_kings) &&
                         (left.with_king == right.with_king)
                );
        }
};

template<>
struct greater_equal<FrisianRules>: public std::binary_function<CaptureValue, CaptureValue, bool>
{
        bool operator()(const CaptureValue& left, const CaptureValue& right) const
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
