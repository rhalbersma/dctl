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
        material_(black_pieces, white_pieces, kings),
        reversible_moves_(0),
        to_move_(to_move)
{       
        hash_index_ = hash::zobrist::Init<Position<Board>, HashIndex>()(*this);

        assert(material_invariant());
}

// the initial position
template<typename Board>
Position<Board> Position<Board>::initial()
{
        return Position<Board>(Board::INITIAL[Side::BLACK], Board::INITIAL[Side::WHITE], 0, Side::WHITE);
}

template<typename Board>
const Position<Board>* Position<Board>::parent() const
{
        return parent_;
}

template<typename Board>
HashIndex Position<Board>::hash_index() const
{
        return hash_index_;
}

template<typename Board>
const Material& Position<Board>::material() const
{
        return material_;
}

template<typename Board>
const Material& Position<Board>::key() const
{
        return material();
}

template<typename Board>
const Restricted& Position<Board>::restricted() const
{
        return restricted_;
}

template<typename Board>
PlyCount Position<Board>::reversible_moves() const
{
        return reversible_moves_;
}

// the side to move
template<typename Board>
bool Position<Board>::to_move() const
{
        return to_move_;
}

// logical consistency of the representation
template<typename Board>
bool Position<Board>::material_invariant() const
{
        return bit::is_within(pieces(), Board::SQUARES);
}

template<typename Board>
bool Position<Board>::hash_index_invariant() const
{
        return (
                hash::zobrist::Find<Position<Board>, HashIndex>()(*this) == 
                hash::zobrist::Init<Position<Board>, HashIndex>()(*this)
        );
}

// black or white men
template<typename Board>
BitBoard Position<Board>::men(bool color) const
{
        return material_.men(color);
}

// black or white kings
template<typename Board>
BitBoard Position<Board>::kings(bool color) const
{
        return material_.kings(color);
}

// black or white pieces
template<typename Board>
BitBoard Position<Board>::pieces(bool color) const
{
        return material_.pieces(color);
}

// black and white men
template<typename Board>
BitBoard Position<Board>::men() const
{
        return material_.men();
}

// black and white kings
template<typename Board>
BitBoard Position<Board>::kings() const
{
        return material_.kings();
}

// black and white pieces
template<typename Board>
BitBoard Position<Board>::pieces() const
{
        return material_.pieces();
}

// unoccupied squares
template<typename Board>
BitBoard not_occupied(const Position<Board>& p)
{
        return Board::SQUARES ^ p.pieces();
}

// men for the side to move
template<typename Board>
BitBoard active_men(const Position<Board>& p)
{
        return p.men(active_color(p));
}

// kings for the side to move
template<typename Board>
BitBoard active_kings(const Position<Board>& p)
{
        return p.kings(active_color(p));
}

// pieces for the side to move
template<typename Board>
BitBoard active_pieces(const Position<Board>& p)
{
        return p.pieces(active_color(p));
}

// the side to move
template<typename Board>
bool active_color(const Position<Board>& p)
{
        return p.to_move();
}

// men for the opposite side
template<typename Board>
BitBoard passive_men(const Position<Board>& p)
{
        return p.men(passive_color(p));
}

// kings for the opposite side
template<typename Board>
BitBoard passive_kings(const Position<Board>& p)
{
        return p.kings(passive_color(p));
}

// pieces for the opposite side
template<typename Board>
BitBoard passive_pieces(const Position<Board>& p)
{
        return p.pieces(passive_color(p));
}

// the opposite side to move
template<typename Board>
bool passive_color(const Position<Board>& p)
{
        return !p.to_move();
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
        if (p.kings(color) && p.men(color) && is_max<Rules>(p.restricted().moves(color)))
                return p.kings(color) ^ p.restricted().king(color);
        else
                return p.kings(color);
}

template<typename Board>
const Position<Board>* grand_parent(const Position<Board>& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<typename Rules, typename Board>
bool is_draw(const Position<Board>& p)
{
        return (
                is_repetition_draw(p) || 
                is_reversible_draw<Rules>(p)
        );
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
