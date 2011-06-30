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
        parent_[0] = &other;
        parent_[1] = other.parent_[0];
}

template<typename Board> template<typename Rules>
void Position<Board>::make(const Pieces& m)
{
        assert(is_pseudo_legal<Rules>(*this, m));

        make_irreversible<Rules>(m);
        make_reversible(m);

        assert(pieces_invariant());
        assert(hash_index_invariant());
}

// tag dispatching based on restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m)
{
        make_irreversible<Rules>(m, Int2Type<variants::is_restricted_consecutive_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_consecutive_same_kings_moves<variants::max_consecutive_same_king_moves<Rules>::value>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<false>)
{
        make_non_conversion_moves(m);
}

template<typename Board>
void Position<Board>::make_non_conversion_moves(const Pieces& m)
{
        if (is_non_conversion(*this, m))
                ++non_conversion_moves_;
        else
                non_conversion_moves_ = 0;
}

template<typename Board> template<PlyCount N>
void Position<Board>::make_consecutive_same_kings_moves(const Pieces& m)
{
        hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());

        if (active_men(*this) && active_kings(*this) && is_non_conversion(*this, m)) {                
                if (bit::is_zero(consecutive_same_kings_ & from_sq(*this, m))) {
                        consecutive_same_kings_ ^= dest_sq(*this, m);
                        consecutive_same_king_moves_[to_move()] = 1;
                } else {
                        if (!is_restricted_king<N>(to_move())) {
                                consecutive_same_kings_ ^= moving_kings(*this, m);
                                ++consecutive_same_king_moves_[to_move()];
                        } else {
                                consecutive_same_kings_ &= passive_kings(*this);
                                consecutive_same_king_moves_[to_move()] = 0;
                        }
                }
        } else {                 
                consecutive_same_kings_ &= passive_kings(*this);
                consecutive_same_king_moves_[to_move()] = 0;
        }

        hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());

        // capture of the opponent's most recently moved king
        if (bit::is_single(consecutive_same_kings_ & captured_pieces(*this, m))) {
                hash_index_ ^= hash::zobrist::Init<Position<Board>, HashIndex>()(*this, !to_move());
                consecutive_same_kings_ &= active_kings(*this);
                consecutive_same_king_moves_[!to_move()] = 0;
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
