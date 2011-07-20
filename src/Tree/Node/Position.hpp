#include <cassert>
#include "../../Hash/Algorithms.h"
#include "../../utils/Bit.h"
#include "../../rules/Traits.h"

namespace tree {
namespace node {

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
        
        hash_index_ = hash::zobrist::Init<node::Position<Board>, HashIndex>()(*this);

        assert(pieces_invariant());
}

// the initial position
template<typename Board>
Position<Board> Position<Board>::initial()
{
        return Position<Board>(Board::INITIAL[Side::BLACK], Board::INITIAL[Side::WHITE], 0, Side::WHITE);
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_draw() const
{
        return is_repetition_draw() || is_reversible_draw<Rules>();
}

template<typename Board>
bool Position<Board>::is_repetition_draw() const
{
        const auto MAX_CYCLE = reversible_moves();

        // a repetition draw needs at least MIN_CYCLE of reversible moves
        if (MAX_CYCLE < MIN_CYCLE)
                return false;

        // find the parent position at MIN_CYCLE ply above the current position
        auto p = grand_parent();
        for (auto i = STRIDE; i != MIN_CYCLE; i += STRIDE)
                p = p->grand_parent();

        // check the hash index of the parent position with the current hash index
        for (auto i = MIN_CYCLE; i <= MAX_CYCLE; i += STRIDE) {
                if (p->hash_index() == hash_index())
                        return true;
                p = p->grand_parent();
        }

        return false;
}

// tag dispatching based on restrictions on consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_reversible_draw() const
{
        return is_reversible_draw<Rules>(Int2Type<rules::is_restricted_reversible_moves<Rules>::value>());
}

// partial specialization for restricted consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_reversible_draw(Int2Type<true>) const
{
        return reversible_moves() >= rules::max_reversible_moves<Rules>::value;
}

// partial specialization for unrestricted consecutive king moves by both sides
template<typename Board> template<typename Rules>
bool Position<Board>::is_reversible_draw(Int2Type<false>) const
{
        return false;
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

// tag dispatching based on restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules>
BitBoard Position<Board>::unrestricted_kings(bool color) const
{
        return unrestricted_kings<Rules>(color, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
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
        if (kings(color) && men(color) && is_restricted_king<rules::max_same_king_moves<Rules>::value>(color))
                return kings(color) ^ same_king(color);
        else
                return kings(color);
}

template<typename Board> template<PlyCount N>
bool Position<Board>::is_restricted_king(bool color) const
{
        return same_king_moves(color) == N;
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
const node::Pieces& Position<Board>::pieces() const
{
        return pieces_;
}

template<typename Board>
const node::Pieces& Position<Board>::key() const
{
        return pieces();
}

template<typename Board>
const node::Position<Board>* Position<Board>::parent() const
{
        return parent_;
}

template<typename Board>
const node::Position<Board>* Position<Board>::grand_parent() const
{
        return parent_ ? parent_->parent() : nullptr;
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
        return hash::zobrist::Find<node::Position<Board>, HashIndex>()(*this) == hash::zobrist::Init<node::Position<Board>, HashIndex>()(*this);
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

}       // namespace node
}       // namespace tree
