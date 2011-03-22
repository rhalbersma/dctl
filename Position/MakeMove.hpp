#include "../Move/Predicates.h"

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
        parents_[0] = &other;
        parents_[1] = other.parents_[0];
}

template<typename Board> template<typename Rules>
void Position<Board>::make(const Pieces& m)
{
        assert(Move::is_pseudo_legal_move<Rules>(*this, m));

        make_irreversible<Rules>(m);
        make_reversible(m);

        assert(pieces_invariant());
        assert(hash_index_invariant());
}

// tag dispatching for restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m)
{
        make_irreversible<Rules>(m, Int2Type<Variants::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_repeated_kings_moves<Variants::max_same_king_moves<Rules>::value>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules>
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<false>)
{
        make_non_conversion(m);
}

template<typename Board>
void Position<Board>::make_non_conversion(const Pieces& m)
{
        if (Move::is_non_conversion(*this, m))
                ++non_conversion_;
        else
                non_conversion_ = 0;
}

template<typename Board> template<PlyCount N>
void Position<Board>::make_repeated_kings_moves(const Pieces& m)
{
        hash_index_ ^= Hash::Zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());

        repeated_kings_ ^= repeated_kings(to_move());
        if (men(to_move()) && kings(to_move()) && Move::is_non_conversion(*this, m)) {
                repeated_kings_ ^= Move::dest_sq(*this, m);                        
                ++repeated_moves_[to_move()];
        } else
                repeated_moves_[to_move()] = 0;

        hash_index_ ^= Hash::Zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());
}

template<typename Board>
void Position<Board>::make_reversible(const Pieces& m)
{
        pieces_ ^= m;
        hash_index_ ^= Hash::Zobrist::Init<Pieces, HashIndex>()(m);

        to_move_ ^= PASS;
        hash_index_ ^= Hash::Zobrist::Init<bool, HashIndex>()();
}
