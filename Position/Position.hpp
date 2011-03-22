#include "../Hash/Algorithms.h"
#include "../Utilities/Bit.h"
#include "../Variants/Rules.h"
#include <cassert>
#include <iostream>

// initialize with a set of bitboards and a color
template<typename Board>
Position<Board>::Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
:
        pieces_(black_pieces, white_pieces, kings),
        repeated_kings_(0),
        non_conversion_(0),
        to_move_(to_move)
{
        repeated_moves_[Side::BLACK] = repeated_moves_[Side::WHITE] = 0;
        parents_[0] = parents_[1] = 0;
        hash_index_ = Hash::Zobrist::Init<Position<Board>, HashIndex>()(*this);
        assert(pieces_invariant());
}

// the initial position
template<typename Board>
Position<Board> Position<Board>::initial(void)
{
        return Position<Board>(Board::INITIAL[Side::BLACK], Board::INITIAL[Side::WHITE], 0, Side::WHITE);
}

template<typename Board>
bool Position<Board>::operator==(const Position<Board>& other) const
{
        return (
		(pieces(Side::BLACK) == other.pieces(Side::BLACK)) &&
		(pieces(Side::WHITE) == other.pieces(Side::WHITE)) &&
                            (kings() == other.kings()) &&
                          (to_move() == other.to_move())
        );
}

template<typename Board>
bool Position<Board>::operator!=(const Position<Board>& other) const
{
        return !(*this == other);
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_draw(void) const
{
        return is_repetition_draw() || is_non_conversion_draw<Rules>();
}

template<typename Board>
bool Position<Board>::is_repetition_draw(void) const
{
        return false;
}

// tag dispatching based on restrictions on consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(void) const
{
        return is_non_conversion_draw<Rules>(Int2Type<Variants::is_restricted_non_conversion_moves<Rules>::value>());
}

// partial specialization for restricted consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(Int2Type<true>) const
{
        return non_conversion() >= Variants::max_non_conversion_moves<Rules>::value;
}

// partial specialization for unrestricted consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(Int2Type<false>) const
{
        return false;
}

template<typename Board> template<PlyCount N>
bool Position<Board>::is_restricted_king(bool color) const
{
        return repeated_moves(color) == N;
}

template<typename Board>
const Pieces& Position<Board>::pieces(void) const
{
        return pieces_;
}

// black and white men
template<typename Board>
BitBoard Position<Board>::men(void) const
{
        return pieces_.men();
}

// black and white kings
template<typename Board>
BitBoard Position<Board>::kings(void) const
{
        return pieces_.kings();
}

// occupied squares
template<typename Board>
BitBoard Position<Board>::occupied(void) const
{
        return pieces_.occupied();
}

// unoccupied squares
template<typename Board>
BitBoard Position<Board>::not_occupied(void) const
{
        return Board::SQUARES ^ occupied();
}

// black or white men
template<typename Board>
BitBoard Position<Board>::men(bool color) const
{
        return pieces_.men(color);
}

// black or white kings
template<typename Board>
BitBoard Position<Board>::kings(bool color) const
{
        return pieces_.kings(color);
}

// black or white pieces
template<typename Board>
BitBoard Position<Board>::pieces(bool color) const
{
        return pieces_.pieces(color);
}

// men for the side to move
template<typename Board>
BitBoard Position<Board>::active_men(void) const
{
        return men(to_move());
}

// kings for the side to move
template<typename Board>
BitBoard Position<Board>::active_kings(void) const
{
        return kings(to_move());
}

// pieces for the side to move
template<typename Board>
BitBoard Position<Board>::active_pieces(void) const
{
        return pieces(to_move());
}

// men for the opposite side
template<typename Board>
BitBoard Position<Board>::passive_men(void) const
{
        return men(!to_move());
}

// kings for the opposite side
template<typename Board>
BitBoard Position<Board>::passive_kings(void) const
{
        return kings(!to_move());
}

// pieces for the opposite side
template<typename Board>
BitBoard Position<Board>::passive_pieces(void) const
{
        return pieces(!to_move());
}

// the side to move
template<typename Board>
bool Position<Board>::to_move(void) const
{
        return to_move_;
}

// tag dispatching for restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color) const
{
        return unrestricted_kings<Rules>(color, Int2Type<Variants::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color, Int2Type<false>) const
{
        return kings(color);
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color, Int2Type<true>) const
{
        if (men(color) && kings(color) && is_restricted_king<Variants::max_same_king_moves<Rules>::value>(color))
                return kings(color) ^ repeated_kings(color);
        else
                return kings(color);
}

template<typename Board>
BitBoard Position<Board>::repeated_kings(void) const
{
        return repeated_kings_;
}

template<typename Board>
BitBoard Position<Board>::repeated_kings(bool color) const
{
        return repeated_kings() & pieces(color);
}

template<typename Board>
PlyCount Position<Board>::repeated_moves(bool color) const
{
        return repeated_moves_[color];
}

template<typename Board>
PlyCount Position<Board>::non_conversion(void) const
{
        return non_conversion_;
}

template<typename Board>
HashIndex Position<Board>::hash_index(void) const
{
        return hash_index_;
}

// logical consistency of the representation
template<typename Board>
bool Position<Board>::pieces_invariant(void) const
{
        return Bit::is_within(occupied(), Board::SQUARES);
}

template<typename Board>
bool Position<Board>::hash_index_invariant(void) const
{
        return Hash::Zobrist::Find<Position<Board>, HashIndex>()(*this) == Hash::Zobrist::Init<Position<Board>, HashIndex>()(*this);
}
