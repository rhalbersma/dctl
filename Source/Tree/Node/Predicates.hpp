#include "Position.h"
#include "Pieces.h"

namespace tree {
namespace node {

template<typename Board>
BitBoard from_sq(const Position<Board>& p, const Pieces& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<typename Board>
BitBoard dest_sq(const Position<Board>& p, const Pieces& m)
{
        return moving_pieces(p, m) & p.not_occupied();
}

template<typename Board>
BitBoard moving_pieces(const Position<Board>& p, const Pieces& m)
{
        return m.pieces(p.to_move());
}

template<typename Board>
BitBoard moving_kings(const Position<Board>& p, const Pieces& m)
{
        return m.kings(p.to_move());
}

template<typename Board>
BitBoard captured_pieces(const Position<Board>& p, const Pieces& m)
{
        return m.pieces(!p.to_move());
}

template<typename Board>
BitBoard captured_kings(const Position<Board>& p, const Pieces& m)
{
        return m.kings(!p.to_move());
}

template<typename Board>
bool is_connected(const Position<Board>& p, const Pieces& m1, const Pieces& m2)
{
        return false;
}

template<typename Board>
bool is_non_conversion(const Position<Board>& p, const Pieces& m)
{
        return is_with_king(p, m) && !is_capture(p, m);
}

template<typename Board>
bool is_promotion(const Position<Board>& p, const Pieces& m)
{
        return bit::is_single(moving_kings(p, m));
}

template<typename Board>
bool is_with_king(const Position<Board>& p, const Pieces& m)
{
        return (moving_kings(p, m) & active_kings(p)) != 0;
}

template<typename Board>
bool is_capture(const Position<Board>& p, const Pieces& m)
{
        return captured_pieces(p, m) != 0;
}

template<typename Rules, typename Board>
bool is_pseudo_legal(const Position<Board>& p, const Pieces& m)
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
bool is_intersecting_capture(const Position<Board>& p, const Pieces& m)
{
        return is_intersecting_capture(p, m, Int2Type<variants::capture_removal<Rules>::value>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Pieces& m, Int2Type<variants::REMOVE_1>)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_men(p));
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool is_intersecting_capture(const Position<Board>&, const Pieces&, Int2Type<variants::REMOVE_N>)
{
        return false;
}

// tag dispatching on capture removal
template<typename Rules>
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces)
{
        return is_intersecting_capture(delta, captured_pieces, Int2Type<variants::capture_removal<Rules>::value>());
}

// tag dispatching based on promotion condition
template<typename Rules>
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta)
{
        return is_intersecting_promotion(promotion, delta, Int2Type<variants::promotion_condition<Rules>::value>());
}

}       // namespace node
}       // namespace tree
