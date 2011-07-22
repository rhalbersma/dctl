#include "Predicates.h"

namespace dctl {

template<typename Board> template<typename Rules>
void Position<Board>::copy_make(const Position<Board>& other, const Move& move)
{
        attach(other);          // attach the position
        make<Rules>(move);      // make the move
}

template<typename Board>
void Position<Board>::attach(const Position<Board>& other)
{
        *this = other;          // copy the position
        parent_ = &other;       // link the pointers
}

template<typename Board> template<typename Rules>
void Position<Board>::make(const Move& m)
{
        assert(is_pseudo_legal<Rules>(*this, m));

        make_irreversible<Rules>(m);
        make_reversible(m);

        assert(pieces_invariant());
        assert(same_king_invariant<Rules>());
        assert(hash_index_invariant());
}

// tag dispatching based on restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Move& m)
{
        make_irreversible<Rules>(m, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Move& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_same_king_moves<Rules>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Move& m, Int2Type<false>)
{
        make_reversible_moves(m);
}

template<typename Board>
void Position<Board>::make_reversible_moves(const Move& m)
{
        if (is_reversible(*this, m))
                ++reversible_moves_;
        else
                reversible_moves_ = 0;
}

template<typename Board> template<typename Rules>
void Position<Board>::make_same_king_moves(const Move& m)
{
        if (active_kings(*this) && active_men(*this)) {
                hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());

                if (is_reversible(*this, m) && !has_restricted_king<Rules>(*this, to_move())) {
                        if (same_king(to_move()) & from_sq(*this, m))
                                ++same_king_moves_[to_move()];               
                        else
                                same_king_moves_[to_move()] = 1;
                        same_king_[to_move()] = dest_sq(*this, m);
                } else {
                        same_king_[to_move()] = 0;
                        same_king_moves_[to_move()] = 0;               
                }

                hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());
        } 

        if (
                same_king_moves(!to_move()) && is_capture(*this, m) &&
                (
                        bit::is_within(same_king(!to_move()), captured_pieces(*this, m)) ||
                        bit::is_within(passive_men(*this)   , captured_pieces(*this, m))
                )
        ) { 
                hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, !to_move());

                same_king_[!to_move()] = 0;
                same_king_moves_[!to_move()] = 0;

                hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, !to_move());
        }
}

template<typename Board>
void Position<Board>::make_reversible(const Move& m)
{
        pieces_ ^= m;
        to_move_ ^= Side::PASS;
        hash_index_ ^= hash::zobrist::Init<Move, HashIndex>()(m);
}

}       // namespace dctl