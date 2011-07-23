#include "Move.h"
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
        make_material(m);

        assert(material_invariant());
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
        if (active_kings(*this) && active_men(*this)) {
                hash_index_ ^= hash::zobrist::Init<Restricted, HashIndex>()(restricted_, to_move());

                if (is_reversible(*this, m) && !is_max<Rules>(restricted_.moves(to_move()))) {
                        if (restricted_.king(to_move()) & from_sq(*this, m))
                                restricted_.increment(to_move(), dest_sq(*this, m));               
                        else
                                restricted_.init(to_move(), dest_sq(*this, m));
                } else {
                        restricted_.reset(to_move());
                }

                hash_index_ ^= hash::zobrist::Init<Restricted, HashIndex>()(restricted_, to_move());
        } 

        if (
                restricted_.moves(!to_move()) && is_capture(*this, m) &&
                (
                        bit::is_within(restricted_.king(!to_move()), captured_pieces(*this, m)) ||
                        bit::is_within(passive_men(*this)          , captured_pieces(*this, m))
                )
        ) {
                hash_index_ ^= hash::zobrist::Init<Restricted, HashIndex>()(restricted_, !to_move());

                restricted_.reset(!to_move());

                hash_index_ ^= hash::zobrist::Init<Restricted, HashIndex>()(restricted_, !to_move());
        }
}

template<typename Board>
void Position<Board>::make_material(const Move& m)
{
        material_ ^= m.material();
        to_move_ ^= m.to_move();
        hash_index_ ^= hash::zobrist::Init<Move, HashIndex>()(m);
}

}       // namespace dctl
