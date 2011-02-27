template<typename Board> template<typename Rules>
void Position<Board>::copy_make(const Position<Board>& p, const Pieces& m)
{
        *this = p;
        link(p);
        make<Rules>(m);
}

template<typename Board> FORCE_INLINE
void Position<Board>::link(const Position<Board>& other)
{
        parent_ = &other;
}

template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make(const Pieces& m)
{
        assert(is_pseudo_legal_move<Rules>(m));

        //make_irreversible<Rules>(m);
        make_reversible(m);

        assert(pieces_invariant());
        assert(hash_index_invariant());
}

// tag dispatching for restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m)
{
        make_irreversible<Rules>(m, Int2Type<Variants::is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_repeated_kings_moves<MaxSameKingMoves<Rules>::VALUE>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<false>)
{
        make_non_conversion(m);
}

template<typename Board> FORCE_INLINE
void Position<Board>::make_non_conversion(const Pieces& m)
{
        if (is_non_conversion(m))
                ++non_conversion_;
        else
                non_conversion_ = 0;
}

template<typename Board> template<PlyCount N> FORCE_INLINE
void Position<Board>::make_repeated_kings_moves(const Pieces& m)
{
        hash_index_ ^= Hash::Zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());

        repeated_kings ^= repeated_kings(to_move());
        if (men(to_move()) && kings(to_move()) && is_non_conversion(m)) {
                repeated_kings ^= king_dest_sq(m);                        
                ++repeated_moves[to_move()];
        } else
                repeated_moves[to_move()] = 0;

        hash_index_ ^= Hash::Zobrist::Init<Position<Board>, HashIndex>()(*this, to_move());
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
bool Position<Board>::is_non_conversion(const Pieces& m) const
{
        return is_with_king(m) && !is_capture(m);
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
        pieces_ ^= m;
        //hash_index_ ^= Hash::Zobrist::Init<Pieces, HashIndex>()(m);

        to_move_ ^= PASS;
        //hash_index_ ^= Hash::Zobrist::Init<bool, HashIndex>()();
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_pseudo_legal_move(const Pieces& m) const
{
        return (
                !Bit::is_multiple(m.pieces(to_move()) & pieces(to_move())) &&           // cannot move multiple pieces                
                Bit::is_within(m.pieces(!to_move()), pieces(!to_move())) &&             // only capture existing pieces
                (                               
                        Bit::is_within(m.kings(!to_move()), kings(!to_move())) ||       // only capture existing kings
                        make_sequential_capture_removal<Rules>(m)                       // exception for Thai draughts
                )
        );
}

// tag dispatching on capture removal
template<typename Board> template<typename Rules>
bool Position<Board>::make_sequential_capture_removal(const Pieces& m) const
{
        return make_sequential_capture_removal(m, Int2Type<Variants::CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool Position<Board>::make_sequential_capture_removal(const Pieces&, Int2Type<Variants::REMOVE_N>) const
{
        return false;
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool Position<Board>::make_sequential_capture_removal(const Pieces& m, Int2Type<Variants::REMOVE_1>) const
{
        // in Thai draughts, a capturing king can land on a square it also captured on
        // if a man was captured on that landing square, the move struct will indicate a king was captured
        return Bit::is_single(m.kings(to_move()) & m.kings(!to_move()) & men(!to_move()));
}
