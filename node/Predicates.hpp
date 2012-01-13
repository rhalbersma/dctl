#pragma once
#include "Position.hpp"
#include "Move.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Board>
BitBoard from_sq(const Position<Board>& p, const Move& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<typename Board>
BitBoard dest_sq(const Position<Board>& p, const Move& m)
{
        return moving_pieces(p, m) & not_occupied(p);
}

template<typename Board>
BitBoard moving_pieces(const Position<Board>& p, const Move& m)
{
        return m.pieces(p.active_color());
}

template<typename Board>
BitBoard moving_kings(const Position<Board>& p, const Move& m)
{
        return m.kings(p.active_color());
}

template<typename Board>
BitBoard captured_pieces(const Position<Board>& p, const Move& m)
{
        return m.pieces(p.passive_color());
}

template<typename Board>
BitBoard captured_kings(const Position<Board>& p, const Move& m)
{
        return m.kings(p.passive_color());
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
                bit::is_subset_of(captured_pieces(p, m), passive_pieces(p)) &&            
                (
                        // only capture existing kings
                        bit::is_subset_of(captured_kings(p, m), passive_kings(p)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture<Rules>(p, m)                            
                )
        );
}

template<typename Rules, typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Move& m)
{
        // tag dispatching on capture removal
        return is_intersecting_capture_dispatch(
                p, m, 
                Int2Type<rules::capture_removal<Rules>::value>()
        );
}

// partial specialization for apres-fini capture removal
template<typename Board>
bool is_intersecting_capture_dispatch(
        const Position<Board>& /* p */, const Move& /* m */, Int2Type<rules::remove_af>
)
{
        return false;
}

// partial specialization for en-passant capture removal
template<typename Board>
bool is_intersecting_capture_dispatch(
        const Position<Board>& p, const Move& m, Int2Type<rules::remove_ep>
)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_men(p));
}

template<typename Rules>
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces)
{
        // tag dispatching on capture removal
        return is_intersecting_capture_dispatch(
                delta, captured_pieces, 
                Int2Type<rules::capture_removal<Rules>::value>()
        );
}

// specialization for apres-fini capture removal
inline bool is_intersecting_capture_dispatch(
        BitBoard /* delta */, BitBoard /* captured_pieces */, Int2Type<rules::remove_af>
)
{
        return false; 
}

// specialization for en-passant capture removal (Thai draughts)
inline bool is_intersecting_capture_dispatch(
        BitBoard delta, BitBoard captured_pieces, Int2Type<rules::remove_ep>
)
{
        // [FEN "W:WK26:B9,12,18,19"] 
        // white has to capture 26x12, landing on a square it also captured on
        return bit::is_single(delta & captured_pieces) && bit::is_multiple(captured_pieces); 
}

template<typename Rules>
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta)
{
        // tag dispatching on promotion condition
        return is_intersecting_promotion_dispatch(
                promotion, delta, 
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

// specialization for en-passant promotion (Russian draughts)
inline bool is_intersecting_promotion_dispatch(
        BitBoard promotion, BitBoard delta, Int2Type<rules::promote_ep>
)
{
        // [FEN "W:W15:B10,13,20,23"] 
        // white has to capture 15x15, promoting on its original square
        return bit::is_single(promotion) && bit::is_zero(delta);
}

// specialization for apres-fini promotion
inline bool is_intersecting_promotion_dispatch(
        BitBoard /* promotion */, BitBoard /* delta */, Int2Type<rules::promote_af>
)
{
        return false;
}

}       // namespace dctl
