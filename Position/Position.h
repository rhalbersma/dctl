#pragma once
#include "../Board/Board.h"
#include "../Rules/Rules.h"
#include "Reversible/Pieces.h"
#include "Reversible/Side.h"
#include "Irreversible/SameKingMoves.h"
#include "Irreversible/NonConversion.h"
#include "Irreversible/Repetition.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename Board = DefaultBoard>
class Position
{
public:
        // constructors
        Position(void);                                         // the initial position
        Position(BitBoard, BitBoard, BitBoard, bool);           // initialize with a set of bitboards and a color

        // predicates
        bool operator==(const Position<Board>&) const;
        bool operator!=(const Position<Board>&) const;
        template<typename> bool is_draw(void) const;

        // views
        const Pieces& pieces(void) const;
        BitBoard men(bool) const;                               // black or white men
        BitBoard kings(bool) const;                             // black or white kings
        BitBoard pieces(bool) const;                            // black or white pieces
        BitBoard men(void) const;                               // black and white men
        BitBoard kings(void) const;                             // black and white kings
        BitBoard occupied(void) const;                          // occupied squares
        BitBoard not_occupied(void) const;                      // unoccupied squares
        Pieces::Composition composition(bool) const;            // composition of black or white pieces
        bool to_move(void) const;                               // the side to move
        BitBoard same_king(bool) const;
        PlyCount same_moves(bool) const;
        PlyCount non_conversion_moves(void) const;

        template<typename> BitBoard unrestricted_kings(bool) const;

        // element access
        HashIndex& hash_index(void);
        HashIndex hash_index(void) const;

        // modifiers
        template<typename> void make(const Pieces&);            // make a move
        template<typename> void undo(const Pieces&);            // undo a move
        void make(void);                                        // make a null move
        void undo(void);                                        // undo a null move

private:
        // implentation
        bool is_repetition_draw(void) const;
        template<typename> bool is_non_conversion_draw(void) const;
        template<typename> bool is_non_conversion_draw(Int2Type<true>) const;
        template<typename> bool is_non_conversion_draw(Int2Type<false>) const;
        template<PlyCount> bool is_restricted_same_king(bool) const;

        template<typename> BitBoard unrestricted_kings(bool, Int2Type<true>) const;
        template<typename> BitBoard unrestricted_kings(bool, Int2Type<false>) const;

        BitBoard king_from_sq(const Pieces&) const;
        BitBoard king_dest_sq(const Pieces&) const;
        bool is_with_king(const Pieces&) const;
        bool is_capture(const Pieces&) const;

        template<typename> void make_irreversible(const Pieces&);
        template<typename> void make_irreversible(const Pieces&, Int2Type<true>);
        template<typename> void make_irreversible(const Pieces&, Int2Type<false>);
        void make_repetition(void);
        void make_non_conversion(const Pieces&);
        template<PlyCount> void make_same_king_moves(const Pieces&);
        void make_reversible(const Pieces&);

        template<typename> void undo_irreversible(void);
        void undo_irreversible(Int2Type<true>);
        void undo_irreversible(Int2Type<false>);
        void undo_repetition(void);
        void undo_non_conversion(void);
        void undo_same_king_moves(void);
        void undo_reversible(const Pieces&);

        // pre-conditions for modifiers
        template<typename> bool is_pseudo_legal_make(const Pieces&) const;
        template<typename> bool is_pseudo_legal_undo(const Pieces&) const;

        template<typename> bool make_sequential_capture_removal(const Pieces&) const;
        bool make_sequential_capture_removal(const Pieces&, Int2Type<REMOVE_1>) const;
        bool make_sequential_capture_removal(const Pieces&, Int2Type<REMOVE_N>) const;
                
        template<typename> bool undo_sequential_capture_removal(const Pieces&) const;
        bool undo_sequential_capture_removal(const Pieces&, Int2Type<REMOVE_1>) const;
        bool undo_sequential_capture_removal(const Pieces&, Int2Type<REMOVE_N>) const;

        // post-conditions for the constructors and modifiers
        bool pieces_invariant(void) const;                      // logical consistency of the representation
        bool hash_index_invariant(void) const;

        // representation
        Pieces d_pieces;
        Side d_side;

        SameKingMoves d_same_king_moves[2];
        NonConversion d_non_conversion;
        Repetition d_repetition; 
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Position.hpp"
#include "MakeUndo.hpp"
