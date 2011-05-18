#pragma once
#include "Pieces.h"
#include "Side.h"
#include "../../Geometry/Board.h"
#include "../../Variants/Rules.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Utilities/CacheAlign.h"

namespace Tree {
namespace Node {

template<typename Board> 
class Position
{
public:
        // constructors
        Position(void) {};                                      // trivial default constructor
        Position(BitBoard, BitBoard, BitBoard, bool);           // initialize with a set of bitboards and a color
        static Position initial(void);                          // initial position

        // predicates
        bool operator==(const Node::Position<Board>&) const;          // overloaded equality operator
        bool operator!=(const Node::Position<Board>&) const;          // overloaded inequality operator
        
        template<typename> 
        bool is_draw(void) const;

        // views
        const Node::Pieces& pieces(void) const;

        BitBoard men(void) const;                               // black and white men
        BitBoard kings(void) const;                             // black and white kings
        BitBoard occupied(void) const;                          // occupied squares
        BitBoard not_occupied(void) const;                      // unoccupied squares
        BitBoard men(bool) const;                               // black or white men
        BitBoard kings(bool) const;                             // black or white kings
        BitBoard pieces(bool) const;                            // black or white pieces
        BitBoard active_men(void) const;                        // men for the side to move
        BitBoard active_kings(void) const;                      // kings for the side to move
        BitBoard active_pieces(void) const;                     // pieces for the side to move
        BitBoard passive_men(void) const;                       // men for the opposite side
        BitBoard passive_kings(void) const;                     // kings for the opposite side
        BitBoard passive_pieces(void) const;                    // pieces for the opposite side
        bool to_move(void) const;                               // side to move

        template<typename> 
        BitBoard unrestricted_kings(bool) const;

        HashIndex hash_index(void) const;
        BitBoard repeated_kings(void) const;
        BitBoard repeated_kings(bool) const;
        PlyCount repeated_moves(bool) const;
        PlyCount non_conversion(void) const;

        // make a move in a copy from another position
        template<typename>
        void copy_make(const Node::Position<Board>&, const Node::Pieces&);
        
        // make a move in the current position
        template<typename>
        void make(const Node::Pieces&);

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

        void link(const Node::Position<Board>&);

        // tag dispatching for restrictions on consecutive moves with the same king
        template<typename> void make_irreversible(const Node::Pieces&);
        template<typename> void make_irreversible(const Node::Pieces&, Int2Type<true >);
        template<typename> void make_irreversible(const Node::Pieces&, Int2Type<false>);

        void make_non_conversion(const Node::Pieces&);
        template<PlyCount> void make_repeated_kings_moves(const Node::Pieces&);
        void make_reversible(const Node::Pieces&);
                
        // post-conditions for the constructors and modifiers
        bool pieces_invariant(void) const;
        bool hash_index_invariant(void) const;

        // representation
        const Node::Position<Board>* parents_[2];
        BitBoard padding_;
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

}       // namespace Node
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Position.hpp"
#include "MakeMove.hpp"
