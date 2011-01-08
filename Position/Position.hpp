#include "../Hash/ZobristHash.h"
#include "../Utilities/Bit.h"
#include "../Rules/Rules.h"
#include <cassert>
#include <iostream>

// the initial position
template<typename Board>
Position<Board>::Position(void)
:
        pieces_(Board::INITIAL[Side::BLACK], Board::INITIAL[Side::WHITE], 0),
        side_(Side::WHITE)
{
        same_king_moves_[Side::BLACK].push_back(BitBoard(), PlyCount());
        same_king_moves_[Side::WHITE].push_back(BitBoard(), PlyCount());
        non_conversion_.push_back(PlyCount());
        repetition_.push_back(ZobristHash<Position<Board>, HashIndex>()(*this));
        assert(pieces_invariant());
}

// initialize with a set of bitboards and a color
template<typename Board>
Position<Board>::Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
:
        pieces_(black_pieces, white_pieces, kings),
        side_(to_move)
{
        same_king_moves_[Side::BLACK].push_back(BitBoard(), PlyCount());
        same_king_moves_[Side::WHITE].push_back(BitBoard(), PlyCount());
        non_conversion_.push_back(PlyCount());
        repetition_.push_back(ZobristHash<Position<Board>, HashIndex>()(*this));
        assert(pieces_invariant());
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
        return repetition_.non_unique_back(non_conversion_moves());
}

// tag dispatching based on restrictions on consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(void) const
{
        return is_non_conversion_draw<Rules>(Int2Type<is_RestrictedNonConversionMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(Int2Type<true>) const
{
        return non_conversion_moves() >= MaxNonConversionMoves<Rules>::VALUE;
}

// partial specialization for unrestricted consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(Int2Type<false>) const
{
        return false;
}

template<typename Board> template<PlyCount N>
bool Position<Board>::is_restricted_same_king_(bool color) const
{
        return same_king_moves_[color].is_restricted<N>();
}

template<typename Board>
const Pieces& Position<Board>::pieces(void) const
{
        return pieces_;
}

// black or white men
template<typename Board>
BitBoard Position<Board>::men(bool color) const
{
        return pieces(color) & ~kings();
}

// black or white kings
template<typename Board>
BitBoard Position<Board>::kings(bool color) const
{
        return pieces(color) & kings();
}

// tag dispatching for restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color) const
{
        return unrestricted_kings<Rules>(color, Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color, Int2Type<true>) const
{
        if (men(color) && kings(color)) {
                if (is_restricted_same_king_<MaxSameKingMoves<Rules>::VALUE>(color))
                        return kings(color) ^ same_king(color);
                else
                        return kings(color);
        } else
                return kings(color);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color, Int2Type<false>) const
{
        return kings(color);
}

// black or white pieces
template<typename Board>
BitBoard Position<Board>::pieces(bool color) const
{
        return pieces_.pieces(color);
}

// black and white men
template<typename Board>
BitBoard Position<Board>::men(void) const
{
        return occupied() & ~kings();
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
	return pieces(Side::BLACK) ^ pieces(Side::WHITE);
}

// unoccupied squares
template<typename Board>
BitBoard Position<Board>::not_occupied(void) const
{
        return Board::SQUARES ^ occupied();
}

// composition of black or white pieces
template<typename Board>
Pieces::Composition Position<Board>::composition(bool color) const
{
        return pieces_.composition(color);
}

// the side to move
template<typename Board>
bool Position<Board>::to_move(void) const
{
        return side_.to_move();
}

template<typename Board>
const SameKingMoves* Position<Board>::same_king_moves(void) const
{
        return same_king_moves_;
}

template<typename Board>
BitBoard Position<Board>::same_king(bool color) const
{
        return same_king_moves_[color].king();
}

template<typename Board>
PlyCount Position<Board>::same_moves(bool color) const
{
        return same_king_moves_[color].moves();
}

template<typename Board>
PlyCount Position<Board>::non_conversion_moves(void) const
{
        return non_conversion_.moves();
}

template<typename Board>
HashIndex& Position<Board>::hash_index(void)
{
        return repetition_.hash_index();
}

template<typename Board>
HashIndex Position<Board>::hash_index(void) const
{
        return repetition_.hash_index();
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
        return hash_index() == ZobristHash<Position<Board>, HashIndex>()(*this);
}
