#pragma once
#include "Pieces.h"
#include "Side.h"
#include "../../Geometry/Board.h"
#include "../../Variants/Rules.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Utilities/CacheAlign.h"

namespace tree {
namespace node {

template<typename Board> 
class Position
{
public:
        // constructors
        Position(void) {};                                      // trivial default constructor
        Position(BitBoard, BitBoard, BitBoard, bool);           // initialize with a set of bitboards and a color
        static Position initial(void);                          // initial position

        // predicates
        template<typename> bool is_draw(void) const;

        // views
        const node::Pieces& pieces(void) const;

        BitBoard men(void) const;                               // black and white men
        BitBoard kings(void) const;                             // black and white kings
        BitBoard occupied(void) const;                          // occupied squares
        BitBoard not_occupied(void) const;                      // unoccupied squares
        BitBoard men(bool) const;                               // black or white men
        BitBoard kings(bool) const;                             // black or white kings
        BitBoard pieces(bool) const;                            // black or white pieces
        bool to_move(void) const;                               // side to move

        template<typename> 
        BitBoard unrestricted_kings(bool) const;

        HashIndex hash_index(void) const;
        BitBoard consecutive_same_kings(void) const;
        BitBoard consecutive_same_kings(bool) const;
        PlyCount consecutive_same_king_moves(bool) const;
        PlyCount non_conversion_moves(void) const;

        const node::Position<Board>* parent(size_t) const;
        const node::Pieces& key(void) const;

        // make a move in a copy from another position
        template<typename> void copy_make(const Position<Board>&, const Pieces&);
        template<typename> void make(const Pieces&);
        
private:
        // implementation
        bool is_repetition_draw(void) const;
        
        // tag dispatching based on restrictions on non conversion moves        
        template<typename> bool is_non_conversion_draw(void) const;
        template<typename> bool is_non_conversion_draw(Int2Type<false>) const;
        template<typename> bool is_non_conversion_draw(Int2Type<true >) const;

        template<PlyCount> bool is_restricted_king(bool) const;

        // tag dispatching based on restrictions on consecutive moves with the same king
        template<typename> BitBoard unrestricted_kings(bool, Int2Type<false>) const;
        template<typename> BitBoard unrestricted_kings(bool, Int2Type<true >) const;

        void link(const Position<Board>&);

        // tag dispatching based on restrictions on consecutive moves with the same king
        template<typename> void make_irreversible(const Pieces&);
        template<typename> void make_irreversible(const Pieces&, Int2Type<false>);
        template<typename> void make_irreversible(const Pieces&, Int2Type<true >);

        void make_non_conversion_moves(const Pieces&);
        template<PlyCount> void make_consecutive_same_kings_moves(const Pieces&);
        void make_reversible(const Pieces&);
                
        // post-conditions for the constructors and modifiers
        bool pieces_invariant(void) const;
        bool hash_index_invariant(void) const;

        // representation
        const node::Position<Board>* parent_[2];
        BitBoard padding_;
        HashIndex hash_index_; 
        Pieces pieces_;
        BitBoard consecutive_same_kings_;
        PlyCount consecutive_same_king_moves_[2];
        PlyCount non_conversion_moves_;
        bool to_move_;

        // implementation
        static const size_t MIN_CYCLE = 4;
        static const size_t STRIDE = 2;
        static_assert(MIN_CYCLE % STRIDE == 0, "Repetition draw needs MIN_CYCLE to be a multiple of STRIDE.");

        static const bool PASS = true;  // toggle the side to move
};

template<typename Board> BitBoard active_men(const Position<Board>&);           // men for the side to move
template<typename Board> BitBoard active_kings(const Position<Board>&);         // kings for the side to move
template<typename Board> BitBoard active_pieces(const Position<Board>&);        // pieces for the side to move
template<typename Board> BitBoard passive_men(const Position<Board>&);          // men for the opposite side
template<typename Board> BitBoard passive_kings(const Position<Board>&);        // kings for the opposite side
template<typename Board> BitBoard passive_pieces(const Position<Board>&);       // pieces for the opposite side

}       // namespace node
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Position.hpp"
#include "MakeMove.hpp"
