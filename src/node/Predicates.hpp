#include "Position.h"

namespace dtl {

template<typename Board>
BitBoard from_sq(const Position<Board>& p, const Move& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<typename Board>
BitBoard dest_sq(const Position<Board>& p, const Move& m)
{
        return moving_pieces(p, m) & p.not_occupied();
}

template<typename Board>
BitBoard moving_pieces(const Position<Board>& p, const Move& m)
{
        return m.pieces(p.to_move());
}

template<typename Board>
BitBoard moving_kings(const Position<Board>& p, const Move& m)
{
        return m.kings(p.to_move());
}

template<typename Board>
BitBoard captured_pieces(const Position<Board>& p, const Move& m)
{
        return m.pieces(!p.to_move());
}

template<typename Board>
BitBoard captured_kings(const Position<Board>& p, const Move& m)
{
        return m.kings(!p.to_move());
}

template<typename Board>
bool is_connected(const Position<Board>& p, const Move& m1, const Move& m2)
{
        return false;
}

template<typename Board>
bool is_reversible(const Position<Board>& p, const Move& m)
{
        return is_with_king(p, m) && !is_capture(p, m);
}

template<typename Board>
bool is_promotion(const Position<Board>& p, const Move& m)
{
        return bit::is_single(moving_kings(p, m));
}

template<typename Board>
bool is_with_king(const Position<Board>& p, const Move& m)
{
        return !bit::is_zero(moving_kings(p, m) & active_kings(p));
}

template<typename Board>
bool is_capture(const Position<Board>& p, const Move& m)
{
        return !bit::is_zero(captured_pieces(p, m));
}

template<typename Rules, typename Board>
bool is_pseudo_legal(const Position<Board>& p, const Move& m)
{
        return (
                // cannot move multiple pieces
                !(bit::is_multiple(from_sq(p, m)) || bit::is_multiple(dest_sq(p, m))) &&
                
                // only capture existing pieces
                bit::is_within(captured_pieces(p, m), passive_pieces(p)) &&            
                (
                        // only capture existing kings
                        bit::is_within(captured_kings(p, m), passive_kings(p)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture<Rules>(p, m)                            
                )
        );
}

// tag dispatching on capture removal
template<typename Rules, typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Move& m)
{
        return is_intersecting_capture(p, m, Int2Type<rules::capture_removal<Rules>::value>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Move& m, Int2Type<rules::REMOVE_1>)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_men(p));
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool is_intersecting_capture(const Position<Board>&, const Move&, Int2Type<rules::REMOVE_N>)
{
        return false;
}

// tag dispatching on capture removal
template<typename Rules>
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces)
{
        return is_intersecting_capture(delta, captured_pieces, Int2Type<rules::capture_removal<Rules>::value>());
}

// tag dispatching based on promotion condition
template<typename Rules>
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta)
{
        return is_intersecting_promotion(promotion, delta, Int2Type<rules::promotion_condition<Rules>::value>());
}

}       // namespace dtl
