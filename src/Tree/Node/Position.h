#pragma once
#include "Pieces.h"
#include "Side.h"
#include "Move.h"
#include "../../utils/IntegerTypes.h"
#include "../../utils/TemplateTricks.h"
#include "../../utils/CacheAlign.h"

namespace tree {

template<typename Board> 
class Position
{
public:
        // constructors
        Position() {};                                          // trivial default constructor
        Position(BitBoard, BitBoard, BitBoard, bool);           // initialize with a set of bitboards and a color
        static Position initial();                              // initial position

        // views
        const Pieces& pieces() const;

        BitBoard men() const;                                   // black and white men
        BitBoard kings() const;                                 // black and white kings
        BitBoard occupied() const;                              // occupied squares
        BitBoard not_occupied() const;                          // unoccupied squares
        BitBoard men(bool) const;                               // black or white men
        BitBoard kings(bool) const;                             // black or white kings
        BitBoard pieces(bool) const;                            // black or white pieces
        bool to_move() const;                                   // side to move

        HashIndex hash_index() const;
        BitBoard same_king(bool) const;
        PlyCount same_king_moves(bool) const;
        PlyCount reversible_moves() const;

        const Position<Board>* parent() const;
        const Position<Board>* grand_parent() const;
        const Pieces& key() const;

        // make a move in a copy from another position
        template<typename> void copy_make(const Position<Board>&, const Move&);
        template<typename> void make(const Move&);
        
private:
        void attach(const Position<Board>&);
        void detach();

        // tag dispatching based on restrictions on consecutive moves with the same king
        template<typename> void make_irreversible(const Move&);
        template<typename> void make_irreversible(const Move&, Int2Type<false>);
        template<typename> void make_irreversible(const Move&, Int2Type<true >);

        void make_reversible_moves(const Move&);
        template<typename> void make_same_king_moves(const Move&);
        void make_reversible(const Move&);
                
        // post-conditions for the constructors and modifiers
        bool pieces_invariant() const;
        bool hash_index_invariant() const;
        template<typename> bool same_king_invariant() const;
        template<typename> bool same_king_invariant(bool) const;
        template<typename> bool same_king_invariant(bool, Int2Type<false>) const;
        template<typename> bool same_king_invariant(bool, Int2Type<true >) const;

        // representation
        const Position<Board>* parent_;
        HashIndex hash_index_; 
        Pieces pieces_;
        BitBoard same_king_[2];
        PlyCount same_king_moves_[2];
        PlyCount reversible_moves_;
        bool to_move_;

        // implementation
        static const bool PASS = true;  // toggle the side to move
};

template<typename Board> BitBoard active_men(const Position<Board>&);           // men for the side to move
template<typename Board> BitBoard active_kings(const Position<Board>&);         // kings for the side to move
template<typename Board> BitBoard active_pieces(const Position<Board>&);        // pieces for the side to move
template<typename Board> BitBoard passive_men(const Position<Board>&);          // men for the opposite side
template<typename Board> BitBoard passive_kings(const Position<Board>&);        // kings for the opposite side
template<typename Board> BitBoard passive_pieces(const Position<Board>&);       // pieces for the opposite side

// tag dispatching based on restrictions on consecutive moves with the same king
template<typename, typename Board> BitBoard unrestricted_kings(const Position<Board>&, bool);
template<typename, typename Board> BitBoard unrestricted_kings(const Position<Board>&, bool, Int2Type<false>);
template<typename, typename Board> BitBoard unrestricted_kings(const Position<Board>&, bool, Int2Type<true >);
template<PlyCount, typename Board> bool has_restricted_king(const Position<Board>&, bool);

template<typename, typename Board> bool is_draw(const Position<Board>&);
template<typename Board> bool is_repetition_draw(const Position<Board>&);       

// tag dispatching based on restrictions on consecutive reversible moves        
template<typename, typename Board> bool is_reversible_draw(const Position<Board>&);
template<typename, typename Board> bool is_reversible_draw(const Position<Board>&, Int2Type<false>);
template<typename, typename Board> bool is_reversible_draw(const Position<Board>&, Int2Type<true >);

}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Position.hpp"
#include "MakeMove.hpp"
