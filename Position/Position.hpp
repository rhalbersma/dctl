#include "../Hash/ZobristHash.h"
#include "../Utilities/Bit.h"
#include "../Rules/Rules.h"
#include <cassert>
#include <iostream>

// the initial position
template<typename Board>
Position<Board>::Position(void)
:
        d_pieces(Board::INITIAL[Side::BLACK], Board::INITIAL[Side::WHITE], 0),
        d_side(Side::WHITE)
{
        d_same_king_moves[Side::BLACK].push_back(BitBoard(), PlyCount());
        d_same_king_moves[Side::WHITE].push_back(BitBoard(), PlyCount());
        d_non_conversion.push_back(PlyCount());
        d_repetition.push_back(ZobristHash<Position<Board>, HashIndex>()(*this));
        assert(pieces_invariant());
}

// initialize with a set of bitboards and a color
template<typename Board>
Position<Board>::Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
:
        d_pieces(black_pieces, white_pieces, kings),
        d_side(to_move)
{
        d_same_king_moves[Side::BLACK].push_back(BitBoard(), PlyCount());
        d_same_king_moves[Side::WHITE].push_back(BitBoard(), PlyCount());
        d_non_conversion.push_back(PlyCount());
        d_repetition.push_back(ZobristHash<Position<Board>, HashIndex>()(*this));
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
        return !d_repetition.unique_back(non_conversion_moves());
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(void) const
{
        return is_non_conversion_draw<Rules>(Int2Type<is_RestrictedNonConversionMoves<Rules>::VALUE>());
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(Int2Type<true>) const
{
        return non_conversion_moves() >= MaxNonConversionMoves<Rules>::VALUE;
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_non_conversion_draw(Int2Type<false>) const
{
        return false;
}

template<typename Board> template<PlyCount N>
bool Position<Board>::is_restricted_same_king(bool color) const
{
        return d_same_king_moves[color].is_restricted<N>();
}

template<typename Board>
const Pieces& Position<Board>::pieces(void) const
{
        return d_pieces;
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

// tag dispatching based on consecutive king move restrictions
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color) const
{
        return unrestricted_kings<Rules>(color, Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive king moves
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color, Int2Type<true>) const
{
        if (men(color) && kings(color)) {
                if (is_restricted_same_king<MaxSameKingMoves<Rules>::VALUE>(color))
                        return kings(color) ^ same_king(color);
                else
                        return kings(color);
        } else
                return kings(color);
}

// partial specialization for unrestricted consecutive king moves
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color, Int2Type<false>) const
{
        return kings(color);
}

// black or white pieces
template<typename Board>
BitBoard Position<Board>::pieces(bool color) const
{
        return d_pieces.pieces(color);
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
        return d_pieces.kings();
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
        return static_cast<Pieces::Composition>((!!kings(color) << 1) + !!men(color));
}

// the side to move
template<typename Board>
bool Position<Board>::to_move(void) const
{
        return d_side.to_move();
}

template<typename Board>
BitBoard Position<Board>::same_king(bool to_move) const
{
        return d_same_king_moves[to_move].king();
}

template<typename Board>
PlyCount Position<Board>::same_moves(bool to_move) const
{
        return d_same_king_moves[to_move].moves();
}

template<typename Board>
PlyCount Position<Board>::non_conversion_moves(void) const
{
        return d_non_conversion.moves();
}

template<typename Board>
HashIndex& Position<Board>::hash_index(void)
{
        return d_repetition.hash_index();
}

template<typename Board>
HashIndex Position<Board>::hash_index(void) const
{
        return d_repetition.hash_index();
}

template<typename Board>
bool Position<Board>::is_pseudo_legal_make(const Pieces& m) const
{
        return (
                !Bit::is_multiple(m.pieces(to_move()) & pieces(to_move())) &&   // cannot move multiple pieces
                Bit::is_within(m.pieces(!to_move()), pieces(!to_move())) &&     // only capture existing pieces
                Bit::is_within(m.kings(!to_move()), kings(!to_move()))          // only capture existing kings
        );
}

template<typename Board>
bool Position<Board>::is_pseudo_legal_undo(const Pieces& m) const
{
        return (
                !Bit::is_multiple(m.pieces(!to_move()) & pieces(!to_move())) && // cannot move multiple pieces
                Bit::is_within(m.pieces(to_move()), ~occupied())                // captured pieces go to empty squares
        );
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

template<typename Board> template<typename Rules>
void Position<Board>::make(const Pieces& m)
{
        assert(is_pseudo_legal_make(m));

        make_irreversible<Rules>(m);
        make_reversible(m);

        assert(pieces_invariant());
        assert(hash_index_invariant());
}

// tag dispatching based on consecutive king moves restrictions
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m)
{
        make_irreversible<Rules>(m, Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive king moves
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_same_king_moves<MaxSameKingMoves<Rules>::VALUE>(m);
}

// partial specialization for unrestricted consecutive king moves
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<false>)
{
        make_repetition();
        make_non_conversion(m);
}

template<typename Board> FORCE_INLINE
void Position<Board>::make_repetition(void)
{
        d_repetition.copy_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::make_non_conversion(const Pieces& m)
{
        d_non_conversion.copy_back();

        if (is_with_king(m) && !is_capture(m))
                d_non_conversion.increment();
        else
                d_non_conversion.reset();
}

template<typename Board> template<PlyCount N> FORCE_INLINE
void Position<Board>::make_same_king_moves(const Pieces& m)
{
        SameKingMoves& current_state = d_same_king_moves[to_move()];
        hash_index() ^= (
                ZobristRandom<Index>::xor_rand(current_state.king(), ZobristRandom<Index>::SAME_KING[to_move()]) ^
                ZobristRandom<Index>::xor_rand(current_state.moves(), ZobristRandom<Index>::SAME_MOVES[to_move()])
        );

        current_state.copy_back();

        if (men(to_move()) && kings(to_move())) {
                const BitBoard from_sq = king_from_sq(m);
                if (from_sq && !is_capture(m)) {
                        if (from_sq != current_state.king())
                                current_state.init(king_dest_sq(m));
                        else {
                                if (!current_state.is_restricted<N>())
                                        current_state.increment(m.pieces(to_move()));
                                else
                                        current_state.reset();
                        }
                } else
                        current_state.reset();
        } else
                current_state.reset();

        hash_index() ^= (
                ZobristRandom<Index>::xor_rand(current_state.king(), ZobristRandom<Index>::SAME_KING[to_move()]) ^
                ZobristRandom<Index>::xor_rand(current_state.moves(), ZobristRandom<Index>::SAME_MOVES[to_move()])
        );
}

template<typename Board>
BitBoard Position<Board>::king_from_sq(const Pieces& m) const
{
        return kings(to_move()) & m.pieces(to_move());
}

template<typename Board>
BitBoard Position<Board>::king_dest_sq(const Pieces& m) const
{
        return ~kings(to_move()) & m.pieces(to_move());
}

template<typename Board>
bool Position<Board>::is_with_king(const Pieces& m) const
{
        return king_from_sq(m) != 0;
}

template<typename Board>
bool Position<Board>::is_capture(const Pieces& m) const
{
        return m.pieces(!to_move()) != 0;
}

template<typename Board> FORCE_INLINE
void Position<Board>::make_reversible(const Pieces& m)
{
        d_pieces.toggle(m);
        hash_index() ^= ZobristHash<Pieces, HashIndex>()(m);

        d_side.pass();
        hash_index() ^= ZobristHash<Side, HashIndex>()();
}

// tag dispatching based on consecutive king moves restrictions
template<typename Board> template<typename Rules>
void Position<Board>::undo(const Pieces& m)
{
        assert(is_pseudo_legal_undo(m));

        undo_reversible(m);
        undo_irreversible<Rules>();
}

// tag dispatching based on consecutive king moves restrictions
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::undo_irreversible(void)
{
        undo_irreversible(Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive king moves
template<typename Board> FORCE_INLINE
void Position<Board>::undo_irreversible(Int2Type<true>)
{
        undo_irreversible(Int2Type<false>());
        undo_same_king_moves();
}

// partial specialization for restricted consecutive king moves
template<typename Board> FORCE_INLINE
void Position<Board>::undo_irreversible(Int2Type<false>)
{
        undo_repetition();
        undo_non_conversion();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_repetition(void)
{
        d_repetition.pop_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_non_conversion(void)
{
        d_non_conversion.pop_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_same_king_moves(void)
{
        d_same_king_moves[to_move()].pop_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_reversible(const Pieces& m)
{
        d_side.pass();
        d_pieces.toggle(m);
}
