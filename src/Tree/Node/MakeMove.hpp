#include "Predicates.h"

namespace tree {
namespace node {

template<typename Board> template<typename Rules>
void Position<Board>::copy_make(const Position<Board>& p, const Pieces& m)
{
        *this = p;
        link(p);
        make<Rules>(m);
}

template<typename Board>
void Position<Board>::link(const Position<Board>& other)
{
        parent_ = &other;
}

template<typename Board> template<typename Rules>
void Position<Board>::make(const Pieces& m)
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
void Position<Board>::make_irreversible(const Pieces& m)
{
        make_irreversible<Rules>(m, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_same_king_moves<rules::max_same_king_moves<Rules>::value>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<false>)
{
        make_reversible_moves(m);
}

template<typename Board>
void Position<Board>::make_reversible_moves(const Pieces& m)
{
        if (is_reversible(*this, m))
                ++reversible_moves_;
        else
                reversible_moves_ = 0;
}

template<typename Board> template<PlyCount N>
void Position<Board>::make_same_king_moves(const Pieces& m)
{
        if (active_kings(*this) && active_men(*this)) {
                hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());

                if (is_reversible(*this, m) && !is_restricted_king<N>(to_move())) {
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
void Position<Board>::make_reversible(const Pieces& m)
{
        pieces_ ^= m;
        hash_index_ ^= hash::zobrist::Init<Pieces, HashIndex>()(m);

        to_move_ ^= PASS;
        hash_index_ ^= hash::zobrist::Init<bool, HashIndex>()();
}

}       // namespace node
}       // namespace tree
