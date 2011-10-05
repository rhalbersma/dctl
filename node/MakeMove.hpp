#include "Move.h"
#include "Predicates.hpp"

namespace dctl {

template<typename Board> template<typename Rules>
void Position<Board>::copy_make(const Position<Board>& other, const Move& move)
{
        *this = other;          // copy the position
        attach(other);          // attach the position
        make<Rules>(move);      // make the move
}

template<typename Board>
void Position<Board>::attach(const Position<Board>& other)
{
        parent_ = &other;       // link the pointers
}

template<typename Board> template<typename Rules>
void Position<Board>::make(const Move& m)
{
        BOOST_ASSERT(is_pseudo_legal<Rules>(*this, m));

        make_irreversible<Rules>(m);
        make_incremental(m);

        BOOST_ASSERT(material_invariant());
        BOOST_ASSERT(hash_index_invariant());
}

// tag dispatching on restrictions on consecutive moves with the same king
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
        make_restricted<Rules>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Move& m, Int2Type<false>)
{
        make_reversible(m);
}

template<typename Board>
void Position<Board>::make_reversible(const Move& m)
{
        if (is_reversible(*this, m))
                ++reversible_moves_;
        else
                reversible_moves_ = 0;
}

template<typename Board> template<typename Rules>
void Position<Board>::make_restricted(const Move& m)
{
        make_active_king_moves<Rules>(m);
        make_passive_king_moves<Rules>(m);                
}

template<typename Board> template<typename Rules>
void Position<Board>::make_active_king_moves(const Move& m)
{
        KingMoves& restricted = restricted_[active_color()];

        if (active_kings(*this) && active_men(*this)) {
                hash_index_ ^= hash::zobrist::Init<KingMoves, HashIndex>()(restricted, active_color());
                if (is_reversible(*this, m) && !is_max<Rules>(restricted.moves())) {
                        if (restricted.king() & from_sq(*this, m))
                                restricted.increment(dest_sq(*this, m));               
                        else
                                restricted.init(dest_sq(*this, m));
                        hash_index_ ^= hash::zobrist::Init<KingMoves, HashIndex>()(restricted, active_color());
                } else
                        restricted.reset();
        } 
}

template<typename Board> template<typename Rules>
void Position<Board>::make_passive_king_moves(const Move& m)
{
        KingMoves& restricted = restricted_[passive_color()];

        if (
                restricted.moves() && is_capture(*this, m) &&
                (
                        bit::is_within(restricted.king() , captured_pieces(*this, m)) ||
                        bit::is_within(passive_men(*this), captured_pieces(*this, m))
                )
        ) {
                hash_index_ ^= hash::zobrist::Init<KingMoves, HashIndex>()(restricted, passive_color());
                restricted.reset();
        }
}

template<typename Board>
void Position<Board>::make_incremental(const Move& m)
{
        material_ ^= m.material();
        to_move_ ^= m.to_move();
        hash_index_ ^= hash::zobrist::Init<Move, HashIndex>()(m);
}

}       // namespace dctl
