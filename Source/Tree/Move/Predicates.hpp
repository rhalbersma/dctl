#include "../Node/Position.h"
#include "../Node/Pieces.h"

namespace Tree {
namespace Move {

template<typename Board>
bool is_connected(const Node::Position<Board>& p, const Node::Pieces& m1, const Node::Pieces& m2)
{
        return false;
}

template<typename Board>
bool is_non_conversion(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return is_with_king(p, m) && !is_capture(p, m);
}

template<typename Board>
bool is_promotion(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return Bit::is_single(moving_kings(p, m));
}

template<typename Board>
bool is_with_king(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return (moving_kings(p, m) & active_kings(p)) != 0;
}

template<typename Board>
bool is_capture(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return captured_pieces(p, m) != 0;
}

template<typename Rules, typename Board>
bool is_pseudo_legal(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return (
                // cannot move multiple pieces
                !(Bit::is_multiple(from_sq(p, m)) || Bit::is_multiple(dest_sq(p, m))) &&
                
                // only capture existing pieces
                Bit::is_within(captured_pieces(p, m), passive_pieces(p)) &&            
                (
                        // only capture existing kings
                        Bit::is_within(captured_kings(p, m), passive_kings(p)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture<Rules>(p, m)                            
                )
        );
}

// tag dispatching on capture removal
template<typename Rules, typename Board>
bool is_intersecting_capture(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return is_intersecting_capture(p, m, Int2Type<Variants::capture_removal<Rules>::value>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool is_intersecting_capture(const Node::Position<Board>& p, const Node::Pieces& m, Int2Type<Variants::REMOVE_1>)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return Bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_men(p));
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool is_intersecting_capture(const Node::Position<Board>&, const Node::Pieces&, Int2Type<Variants::REMOVE_N>)
{
        return false;
}

// tag dispatching on capture removal
template<typename Rules>
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces)
{
        return is_intersecting_capture(delta, captured_pieces, Int2Type<Variants::capture_removal<Rules>::value>());
}

// tag dispatching based on promotion condition
template<typename Rules>
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta)
{
        return is_intersecting_promotion(promotion, delta, Int2Type<Variants::promotion_condition<Rules>::value>());
}

template<typename Board>
BitBoard from_sq(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<typename Board>
BitBoard dest_sq(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return moving_pieces(p, m) & p.not_occupied();
}

template<typename Board>
BitBoard moving_pieces(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return m.pieces(p.to_move());
}

template<typename Board>
BitBoard moving_kings(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return m.kings(p.to_move());
}

template<typename Board>
BitBoard captured_pieces(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return m.pieces(!p.to_move());
}

template<typename Board>
BitBoard captured_kings(const Node::Position<Board>& p, const Node::Pieces& m)
{
        return m.kings(!p.to_move());
}

}       // namespace Move
}       // namespace Tree
