#pragma once
#include "Material.h"
#include "Restricted.h"
#include "Side.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

class Move;

template<typename Board> 
class Position
{
public:
        // constructors
        Position() {};                                          // trivial default constructor
        Position(BitBoard, BitBoard, BitBoard, bool);           // initialize with a set of bitboards and a color
        static Position initial();                              // initial position

        // views
        const Position<Board>* parent() const;
        HashIndex hash_index() const;
        const Material& material() const;
        const Restricted& restricted() const;
        const KingMoves& restricted(bool) const;
        PlyCount reversible_moves() const;
        bool active_color() const;                              // side to move
        bool passive_color() const;                             // opposite side to move

        const Material& key() const;

        BitBoard men(bool) const;                               // black or white men
        BitBoard kings(bool) const;                             // black or white kings
        BitBoard pieces(bool) const;                            // black or white pieces
        BitBoard men() const;                                   // black and white men
        BitBoard kings() const;                                 // black and white kings
        BitBoard pieces() const;                                // black and white pieces

        // make a move in a copy from another position
        template<typename> void copy_make(const Position<Board>&, const Move&);
        template<typename> void make(const Move&);
        void attach(const Position<Board>&);
        
private:
        // implementation
        void detach();

        // tag dispatching based on restrictions on consecutive moves with the same king
        template<typename> void make_irreversible(const Move&);
        template<typename> void make_irreversible(const Move&, Int2Type<false>);
        template<typename> void make_irreversible(const Move&, Int2Type<true >);

        void make_reversible(const Move&);
        template<typename> void make_restricted(const Move&);
        template<typename> void make_active_king_moves(const Move&);
        template<typename> void make_passive_king_moves(const Move&);
        void make_incremental(const Move&);
                
        // post-conditions for the constructors and modifiers
        bool material_invariant() const;
        bool hash_index_invariant() const;

        // representation
        const Position<Board>* parent_;
        HashIndex hash_index_; 
        Material material_;
        Restricted restricted_;
        PlyCount reversible_moves_;
        bool to_move_;
};

template<typename Board> BitBoard not_occupied(const Position<Board>&);         // unoccupied squares

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

template<typename Board> const Position<Board>* grand_parent(const Position<Board>&);

template<typename Board> const KingMoves& active_restricted(const Position<Board>&);
template<typename Board> const KingMoves& passive_restricted(const Position<Board>&);

template<typename, typename Board> bool is_draw(const Position<Board>&);
template<typename Board> bool is_repetition_draw(const Position<Board>&);       

// tag dispatching based on restrictions on consecutive reversible moves        
template<typename, typename Board> bool is_reversible_draw(const Position<Board>&);
template<typename, typename Board> bool is_reversible_draw(const Position<Board>&, Int2Type<false>);
template<typename, typename Board> bool is_reversible_draw(const Position<Board>&, Int2Type<true >);

template<typename Board> int state(const Position<Board>&);
template<bool, typename Board> int state(const Position<Board>&);
int state(bool, BitBoard, BitBoard);

}       // namespace dctl

// include template definitions inside header
#include "Position.hpp"
#include "MakeMove.hpp"
