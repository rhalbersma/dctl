#include "../Position/Position.h"
#include "../Position/Pieces.h"

namespace Move {

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
        return Bit::is_single(moving_kings(p, m));
}

template<typename Board>
bool is_with_king(const Position<Board>& p, const Pieces& m)
{
        return (moving_kings(p, m) & p.active_kings()) != 0;
}

template<typename Board>
bool is_capture(const Position<Board>& p, const Pieces& m)
{
        return captured_pieces(p, m) != 0;
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
BitBoard from_sq(const Position<Board>& p, const Pieces& m)
{
        return moving_pieces(p, m) & p.active_pieces();
}

template<typename Board>
BitBoard dest_sq(const Position<Board>& p, const Pieces& m)
{
        return moving_pieces(p, m) & p.not_occupied();
}

template<typename Rules, typename Board>
bool is_pseudo_legal(const Position<Board>& p, const Pieces& m)
{
        return (
                !Bit::is_multiple(from_sq(p, m)) &&                                     // cannot move multiple pieces                
                Bit::is_within(captured_pieces(p, m), p.passive_pieces()) &&            // only capture existing pieces
                (                               
                        Bit::is_within(captured_kings(p, m), p.passive_kings()) ||      // only capture existing kings
                        is_sequential_capture_removal<Rules>(p, m)                      // exception for Thai draughts
                )
        );
}

// tag dispatching on capture removal
template<typename Rules, typename Board>
bool is_sequential_capture_removal(const Position<Board>& p, const Pieces& m)
{
        return is_sequential_capture_removal(p, m, Int2Type<Variants::capture_removal<Rules>::value>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool is_sequential_capture_removal(const Position<Board>& p, const Pieces& m, Int2Type<Variants::REMOVE_1>)
{
        // in Thai draughts, a capturing king can land on a square it also captured on
        // if a man was captured on that landing square, the move struct will indicate a king was captured
        return Bit::is_single(moving_kings(p, m) & captured_kings(p, m) & p.passive_men());
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool is_sequential_capture_removal(const Position<Board>& p, const Pieces&, Int2Type<Variants::REMOVE_N>)
{
        return false;
}

// tag dispatching on capture removal
template<typename Rules>
bool is_sequential_capture_removal(BitBoard delta, BitBoard captured_pieces)
{
        return sequential_capture_removal(delta, captured_pieces, Int2Type<Variants::capture_removal<Rules>::value>());
}

// tag dispatching based on promotion condition
template<typename Rules>
bool is_promotion_en_passant(BitBoard promotion, BitBoard delta)
{
        return promotion_en_passant(promotion, delta, Int2Type<Variants::promotion_condition<Rules>::value>());
}

}       // namespace Move
