#include "CaptureValue.h"
#include "Rules.h"
#include "../Utilities/Bit.h"

// tag dispatching on quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const CaptureValue& capture, BitBoard captured_pieces) const
{
        return large<Rules>()(capture, captured_pieces, Int2Type<is_MajorityPrecedence<Rules>::VALUE>());
}

// partial specialization for no quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const CaptureValue&, BitBoard captured_pieces, Int2Type<false>) const
{
        return Bit::count(captured_pieces) >= LargeCaptureValue<Rules>::VALUE;
}

// partial specialization for quantity capture precedence
template<typename Rules>
bool large<Rules>::operator()(const CaptureValue& capture, BitBoard, Int2Type<true>) const
{
        return capture.num_pieces >= LargeCaptureValue<Rules>::VALUE;
}

//+----------------------------------------------------------------------------+

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

//+----------------------------------------------------------------------------+

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

//+----------------------------------------------------------------------------+

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

//+----------------------------------------------------------------------------+

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

//+----------------------------------------------------------------------------+

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
