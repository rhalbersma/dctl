template<typename Board> template<typename Rules>
void Position<Board>::make(const Pieces& m)
{
        assert(is_pseudo_legal_make<Rules>(m));

        //make_irreversible<Rules>(m);
        make_reversible(m);

        assert(pieces_invariant());
        assert(hash_index_invariant());
}

// tag dispatching for restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m)
{
        make_irreversible<Rules>(m, Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<true>)
{
        make_irreversible<Rules>(m, Int2Type<false>());
        make_same_king_moves<MaxSameKingMoves<Rules>::VALUE>(m);
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::make_irreversible(const Pieces& m, Int2Type<false>)
{
        make_repetition();
        make_non_conversion(m);
}

template<typename Board> FORCE_INLINE
void Position<Board>::make_repetition(void)
{
        repetition_.copy_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::make_non_conversion(const Pieces& m)
{
        non_conversion_.copy_back();

        if (is_with_king(m) && !is_capture(m))
                non_conversion_.increment();
        else
                non_conversion_.reset();
}

template<typename Board> template<PlyCount N> FORCE_INLINE
void Position<Board>::make_same_king_moves(const Pieces& m)
{
        SameKingMoves& state = same_king_moves_[to_move()];
        state.copy_back();

        hash_index() ^= ZobristHash<SameKingMoves, HashIndex>()(state, to_move());

        if (men(to_move()) && kings(to_move())) {
                const BitBoard from_sq = king_from_sq(m);
                if (from_sq && !is_capture(m)) {
                        if (from_sq != state.king())
                                state.init(king_dest_sq(m));
                        else {
                                if (!state.is_restricted<N>())
                                        state.increment(m.pieces(to_move()));
                                else
                                        state.reset();
                        }
                } else
                        state.reset();
        } else
                state.reset();

        hash_index() ^= ZobristHash<SameKingMoves, HashIndex>()(state, to_move());
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
        pieces_.toggle(m);
        //hash_index() ^= ZobristHash<Pieces, HashIndex>()(m);

        side_.pass();
        //hash_index() ^= ZobristHash<Side, HashIndex>()();
}

template<typename Board> template<typename Rules>
void Position<Board>::undo(const Pieces& m)
{
        assert(is_pseudo_legal_undo<Rules>(m));

        undo_reversible(m);
        //undo_irreversible<Rules>();
}

// tag dispatching for restrictions on consecutive moves with the same king
template<typename Board> template<typename Rules> FORCE_INLINE
void Position<Board>::undo_irreversible(void)
{
        undo_irreversible(Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive moves with the same king
template<typename Board> FORCE_INLINE
void Position<Board>::undo_irreversible(Int2Type<true>)
{
        undo_irreversible(Int2Type<false>());
        undo_same_king_moves();
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Board> FORCE_INLINE
void Position<Board>::undo_irreversible(Int2Type<false>)
{
        undo_repetition();
        undo_non_conversion();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_repetition(void)
{
        repetition_.pop_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_non_conversion(void)
{
        non_conversion_.pop_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_same_king_moves(void)
{
        same_king_moves_[to_move()].pop_back();
}

template<typename Board> FORCE_INLINE
void Position<Board>::undo_reversible(const Pieces& m)
{
        side_.pass();
        pieces_.toggle(m);
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_pseudo_legal_make(const Pieces& m) const
{
        return (
                !Bit::is_multiple(m.pieces(to_move()) & pieces(to_move())) &&           // cannot move multiple pieces                
                Bit::is_within(m.pieces(!to_move()), pieces(!to_move())) &&             // capture existing pieces
                (                               
                        Bit::is_within(m.kings(!to_move()), kings(!to_move())) ||       // capture existing kings
                        make_sequential_capture_removal<Rules>(m)                       // Thai draughts
                )
        );
}

// tag dispatching on capture removal
template<typename Board> template<typename Rules>
bool Position<Board>::make_sequential_capture_removal(const Pieces& m) const
{
        return make_sequential_capture_removal(m, Int2Type<CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool Position<Board>::make_sequential_capture_removal(const Pieces&, Int2Type<REMOVE_N>) const
{
        return false;
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool Position<Board>::make_sequential_capture_removal(const Pieces& m, Int2Type<REMOVE_1>) const
{
        // in Thai draughts, a capturing king can land on a square it also captured on
        // if a man was captured on that landing square, the move struct will indicate a king was captured
        return Bit::is_single(m.kings(to_move()) & m.kings(!to_move()) & men(!to_move()));
}

template<typename Board> template<typename Rules>
bool Position<Board>::is_pseudo_legal_undo(const Pieces& m) const
{
        return (
                !Bit::is_multiple(m.pieces(!to_move()) & pieces(!to_move())) && // cannot move multiple pieces
                (
                        Bit::is_within(m.pieces(to_move()), not_occupied()) ||  // captured pieces go to empty squares
                        undo_sequential_capture_removal<Rules>(m)               // Thai draughts
                )
        );
}

// tag dispatching on capture removal
template<typename Board> template<typename Rules>
bool Position<Board>::undo_sequential_capture_removal(const Pieces& m) const
{
        return undo_sequential_capture_removal(m, Int2Type<CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for complete removal after a capture sequence
template<typename Board>
bool Position<Board>::undo_sequential_capture_removal(const Pieces&, Int2Type<REMOVE_N>) const
{
        return false;
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Board>
bool Position<Board>::undo_sequential_capture_removal(const Pieces& m, Int2Type<REMOVE_1>) const
{
        // in Thai draughts, a capturing king can land on a square it also captured on
        // if a man was captured on that landing square, the move struct will indicate a king was captured
        return Bit::is_single(m.kings(to_move()) & m.kings(!to_move()) & kings(!to_move()));
}
