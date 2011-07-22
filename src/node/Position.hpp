#include <cassert>
#include "../hash/Algorithms.h"
#include "../utils/Bit.h"
#include "../rules/Rules.h"

namespace dctl {

// initialize with a set of bitboards and a color
template<typename Board>
Position<Board>::Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
:
        parent_(nullptr), 
        pieces_(black_pieces, white_pieces, kings),
        reversible_moves_(0),
        to_move_(to_move)
{
        same_king_[Side::BLACK] = same_king_[Side::WHITE] = 0;
        same_king_moves_[Side::BLACK] = same_king_moves_[Side::WHITE] = 0;
        
        hash_index_ = hash::zobrist::Init<Position<Board>, HashIndex>()(*this);

        assert(pieces_invariant());
}

// the initial position
template<typename Board>
Position<Board> Position<Board>::initial()
{
        return Position<Board>(Board::INITIAL[Side::BLACK], Board::INITIAL[Side::WHITE], 0, Side::WHITE);
}

// black and white men
template<typename Board>
BitBoard Position<Board>::men() const
{
        return pieces_.men();
}

// black and white kings
template<typename Board>
BitBoard Position<Board>::kings() const
{
        return pieces_.kings();
}

// occupied squares
template<typename Board>
BitBoard Position<Board>::occupied() const
{
        return pieces_.occupied();
}

// unoccupied squares
template<typename Board>
BitBoard Position<Board>::not_occupied() const
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

// the side to move
template<typename Board>
bool Position<Board>::to_move() const
{
        return to_move_;
}

template<typename Board>
BitBoard Position<Board>::same_king(bool color) const
{
        return same_king_[color];
}

template<typename Board>
PlyCount Position<Board>::same_king_moves(bool color) const
{
        return same_king_moves_[color];
}

template<typename Board>
PlyCount Position<Board>::reversible_moves() const
{
        return reversible_moves_;
}

template<typename Board>
HashIndex Position<Board>::hash_index() const
{
        return hash_index_;
}

template<typename Board>
const Pieces& Position<Board>::pieces() const
{
        return pieces_;
}

template<typename Board>
const Pieces& Position<Board>::key() const
{
        return pieces();
}

template<typename Board>
const Position<Board>* Position<Board>::parent() const
{
        return parent_;
}

// logical consistency of the representation
template<typename Board>
bool Position<Board>::pieces_invariant() const
{
        return bit::is_within(occupied(), Board::SQUARES);
}

template<typename Board>
bool Position<Board>::hash_index_invariant() const
{
        return (
                hash::zobrist::Find<Position<Board>, HashIndex>()(*this) == 
                hash::zobrist::Init<Position<Board>, HashIndex>()(*this)
        );
}

template<typename Board> template<typename Rules>
bool Position<Board>::same_king_invariant() const
{
        return (
                same_king_invariant<Rules>(Side::BLACK) && 
                same_king_invariant<Rules>(Side::WHITE)
        );
}

template<typename Board> template<typename Rules>
bool Position<Board>::same_king_invariant(bool color) const
{
        return same_king_invariant<Rules>(color, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
}

template<typename Board> template<typename Rules>
bool Position<Board>::same_king_invariant(bool color, Int2Type<false>) const
{
        return (
                same_king(color) == 0 && 
                same_king_moves(color) == 0
        );
}

template<typename Board> template<typename Rules>
bool Position<Board>::same_king_invariant(bool color, Int2Type<true>) const
{
        return (
                !bit::is_multiple(same_king(color)) && 
                bit::is_within(same_king(color), kings(color)) &&
                bit::is_exclusive(same_king(Side::BLACK), same_king(Side::WHITE)) &&
                same_king_moves(color) <= rules::max_same_king_moves<Rules>::value &&
                (same_king(color) == 0) == (same_king_moves(color) == 0)
        );
}

// men for the side to move
template<typename Board>
BitBoard active_men(const Position<Board>& p)
{
        return p.men(p.to_move());
}

// kings for the side to move
template<typename Board>
BitBoard active_kings(const Position<Board>& p)
{
        return p.kings(p.to_move());
}

// pieces for the side to move
template<typename Board>
BitBoard active_pieces(const Position<Board>& p)
{
        return p.pieces(p.to_move());
}

// men for the opposite side
template<typename Board>
BitBoard passive_men(const Position<Board>& p)
{
        return p.men(!p.to_move());
}

// kings for the opposite side
template<typename Board>
BitBoard passive_kings(const Position<Board>& p)
{
        return p.kings(!p.to_move());
}

// pieces for the opposite side
template<typename Board>
BitBoard passive_pieces(const Position<Board>& p)
{
        return p.pieces(!p.to_move());
}

// tag dispatching based on restrictions on consecutive moves with the same king
template<typename Rules, typename Board>
BitBoard unrestricted_kings(const Position<Board>& p, bool color)
{
        return unrestricted_kings<Rules>(p, color, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Rules, typename Board>
BitBoard unrestricted_kings(const Position<Board>& p, bool color, Int2Type<false>)
{
        return p.kings(color);
}

// partial specialization for restricted consecutive moves with the same king
template<typename Rules, typename Board>
BitBoard unrestricted_kings(const Position<Board>& p, bool color, Int2Type<true>)
{
        return p.kings(color) ^ p.same_king(color);
}

template<typename Rules, typename Board>
bool has_restricted_king(const Position<Board>& p, bool color)
{
        return p.same_king_moves(color) == rules::max_same_king_moves<Rules>::value;
}

template<typename Board>
const Position<Board>* grand_parent(const Position<Board>& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<typename Rules, typename Board>
bool is_draw(const Position<Board>& p)
{
        return 
                is_repetition_draw(p) || 
                is_reversible_draw<Rules>(p)
        ;
}

template<typename Board>
bool is_repetition_draw(const Position<Board>& p)
{
        // a repetition draw needs at least 4 reversible moves
        if (p.reversible_moves() < 4)
                return false;

        // find the parent position at 4 ply above the current position
        auto q = grand_parent(*grand_parent(p));

        // compare the ancestor hash indices with the current hash index
        for (auto i = 4; i <= p.reversible_moves(); i += 2) {
                if (q->hash_index() == p.hash_index())
                        return true;
                q = grand_parent(*q);
        }
        return false;
}

// tag dispatching based on restrictions on consecutive king moves by both sides
template<typename Rules, typename Board>
bool is_reversible_draw(const Position<Board>& p)
{
        return is_reversible_draw<Rules>(p, Int2Type<rules::is_restricted_reversible_moves<Rules>::value>());
}

// partial specialization for unrestricted consecutive king moves by both sides
template<typename Rules, typename Board>
bool is_reversible_draw(const Position<Board>&, Int2Type<false>)
{
        return false;
}

// partial specialization for restricted consecutive king moves by both sides
template<typename Rules, typename Board>
bool is_reversible_draw(const Position<Board>& p, Int2Type<true>)
{
        return p.reversible_moves() >= rules::max_reversible_moves<Rules>::value;
}

}       // namespace dctl
