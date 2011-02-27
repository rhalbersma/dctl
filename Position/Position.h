#pragma once
#include "../Geometry/Board.h"
#include "../Variants/Rules.h"
#include "Pieces.h"
#include "Side.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Utilities/CacheAlign.h"

template<typename Board> 
class Position
{
public:
        // constructors
        Position(void) {};                                      // trivial default constructor
        Position(BitBoard, BitBoard, BitBoard, bool);           // initialize with a set of bitboards and a color
        static Position initial(void);                          // initial position

        // predicates
        bool operator==(const Position<Board>&) const;          // overloaded equality operator
        bool operator!=(const Position<Board>&) const;          // overloaded inequality operator
        
        template<typename> 
        bool is_draw(void) const;

        // views
        const Pieces& pieces(void) const;

        BitBoard men(void) const;                               // black and white men
        BitBoard kings(void) const;                             // black and white kings
        BitBoard occupied(void) const;                          // occupied squares
        BitBoard not_occupied(void) const;                      // unoccupied squares
        BitBoard men(bool) const;                               // black or white men
        BitBoard kings(bool) const;                             // black or white kings
        BitBoard pieces(bool) const;                            // black or white pieces
        bool to_move(void) const;                               // side to move

        HashIndex hash_index(void) const;
        BitBoard repeated_kings(void) const;
        BitBoard repeated_kings(bool) const;
        PlyCount repeated_moves(bool) const;
        PlyCount non_conversion(void) const;

        template<typename> 
        BitBoard unrestricted_kings(bool) const;

        // make a move in a copy from another position
        template<typename>
        void copy_make(const Position<Board>&, const Pieces&);
        
        // make a move in the current position
        template<typename>
        void make(const Pieces&);

private:
        // implentation
        bool is_repetition_draw(void) const;
        
        // tag dispatching based on restrictions on consecutive king moves by both sides        
        template<typename> bool is_non_conversion_draw(void) const;
        template<typename> bool is_non_conversion_draw(Int2Type<true>) const;
        template<typename> bool is_non_conversion_draw(Int2Type<false>) const;

        template<PlyCount> bool is_restricted_king(bool) const;

        // tag dispatching for restrictions on consecutive moves with the same king
        template<typename> BitBoard unrestricted_kings(bool, Int2Type<true>) const;
        template<typename> BitBoard unrestricted_kings(bool, Int2Type<false>) const;

        BitBoard king_from_sq(const Pieces&) const;
        BitBoard king_dest_sq(const Pieces&) const;
        bool is_non_conversion(const Pieces&) const;
        bool is_with_king(const Pieces&) const;
        bool is_capture(const Pieces&) const;

        void link(const Position<Board>&);

        // tag dispatching for restrictions on consecutive moves with the same king
        template<typename> void make_irreversible(const Pieces&);
        template<typename> void make_irreversible(const Pieces&, Int2Type<true>);
        template<typename> void make_irreversible(const Pieces&, Int2Type<false>);

        void make_non_conversion(const Pieces&);
        template<PlyCount> void make_repeated_kings_moves(const Pieces&);
        void make_reversible(const Pieces&);

        // pre-conditions for modifiers
        template<typename> bool is_pseudo_legal_move(const Pieces&) const;
        template<typename> bool is_pseudo_legal_undo(const Pieces&) const;

        // tag dispatching on capture removal
        template<typename> bool make_sequential_capture_removal(const Pieces&) const;
        bool make_sequential_capture_removal(const Pieces&, Int2Type<Variants::REMOVE_1>) const;
        bool make_sequential_capture_removal(const Pieces&, Int2Type<Variants::REMOVE_N>) const;
                
        // post-conditions for the constructors and modifiers
        bool pieces_invariant(void) const;
        bool hash_index_invariant(void) const;

        // representation
        const Position<Board>* parent_;
        const Position<Board>* padding_;
        BitBoard pad_;
        HashIndex hash_index_; 
        Pieces pieces_;
        BitBoard repeated_kings_;
        PlyCount repeated_moves_[2];
        PlyCount non_conversion_;
        bool to_move_;

        // implementation
        const static size_t MIN_CYCLE = 4;
        const static size_t STRIDE = 2;
        static const bool PASS = true;  // toggle the side to move

};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Position.hpp"
#include "MakeMove.hpp"
