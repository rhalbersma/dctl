#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <functional>                           // std::unary_function
#include "Move.hpp"
#include "Restricted.hpp"
#include "Side.hpp"
#include "../bit/Bit.hpp"
#include "../hash/zobrist/Init.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

template<typename Board> 
struct Position
{
public:
        Position() 
        { 
                /* no-op */ 
        }

        // initialize with a set of bitboards and a color
        Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
        :
                parent_(nullptr),
                material_(black_pieces, white_pieces, kings),
                reversible_moves_(0),
                to_move_(to_move)
        {       
                hash_index_ = hash::zobrist::Init<Position<Board>, HashIndex>()(*this);
                BOOST_ASSERT(material_invariant());
        }           

        // initial position
        static Position initial()
        {
                return Position<Board>(Board::INITIAL[Side::black], Board::INITIAL[Side::white], 0, Side::white);
        }                              

        // views
        const Position<Board>* parent() const
        {
                return parent_;
        }

        HashIndex hash_index() const
        {
                return hash_index_;
        }

        const Move& material() const
        {
                return material_;
        }

        const Restricted& restricted() const
        {
                return restricted_;
        }

        const KingMoves& restricted(bool color) const
        {
                return restricted_[color];
        }

        PlyCount reversible_moves() const
        {
                return reversible_moves_;
        }

        // side to move
        bool active_color() const
        {
                return to_move_;
        }
        
        // opposite side to move
        bool passive_color() const
        {
                return !to_move_;
        }                           

        const Move& key() const
        {
                return material();
        }

        // black or white men
        BitBoard men(bool color) const
        {
                return material_.men(color);
        }                               
 
        // black or white kings
        BitBoard kings(bool color) const
        {
                return material_.kings(color);
        }                            
                
        // black or white pieces
        BitBoard pieces(bool color) const
        {
                return material_.pieces(color);
        }                           
        
        // black and white men
        BitBoard men() const
        {
                return material_.men();
        }                                  
        
        // black and white kings
        BitBoard kings() const
        {
                return material_.kings();
        }                                
        
        // black and white pieces
        BitBoard pieces() const
        {
                return material_.pieces();
        }                               

        // make a move in a copy from another position
        template<typename> 
        void copy_make(const Position<Board>&, const Move&);
        
        template<typename> 
        void make(const Move&);
        
        void attach(const Position<Board>&);
        
private:
        // implementation
        void detach();

        // tag dispatching on restrictions on consecutive moves with the same king
        template<typename> void make_irreversible(const Move&);
        template<typename> void make_irreversible(const Move&, Int2Type<false>);
        template<typename> void make_irreversible(const Move&, Int2Type<true >);

        void make_reversible(const Move&);
        template<typename> void make_restricted(const Move&);
        template<typename> void make_active_king_moves(const Move&);
        template<typename> void make_passive_king_moves(const Move&);
        void make_incremental(const Move&);
                
        // post-conditions for the constructors and modifiers
        bool material_invariant() const
        {
                return bit::is_within(pieces(), Board::squares);
        }

        bool hash_index_invariant() const
        {
                return (
                        hash::zobrist::Find<Position<Board>, HashIndex>()(*this) == 
                        hash::zobrist::Init<Position<Board>, HashIndex>()(*this)
                );
        }

        // representation
        const Position<Board>* parent_;
        HashIndex hash_index_; 
        Move material_;
        Restricted restricted_;
        PlyCount reversible_moves_;
        bool to_move_;
};

// unoccupied squares
template<typename Board> 
BitBoard not_occupied(const Position<Board>& p)
{
        return Board::squares ^ p.pieces();
}         

// men for the side to move
template<typename Board> 
BitBoard active_men(const Position<Board>& p)
{
        return p.men(p.active_color());
}           

// kings for the side to move
template<typename Board> 
BitBoard active_kings(const Position<Board>& p)
{
        return p.kings(p.active_color());
}         

// pieces for the side to move
template<typename Board> 
BitBoard active_pieces(const Position<Board>& p)
{
        return p.pieces(p.active_color());
}       

// men for the opposite side
template<typename Board> 
BitBoard passive_men(const Position<Board>& p)
{
        return p.men(p.passive_color());
}        

// kings for the opposite side
template<typename Board> 
BitBoard passive_kings(const Position<Board>& p)
{
        return p.kings(p.passive_color());
}      

// pieces for the opposite side
template<typename Board> 
BitBoard passive_pieces(const Position<Board>& p)
{
        return p.pieces(p.passive_color());
}       

// tag dispatching on restrictions on consecutive moves with the same king
template<typename Rules, typename Board> 
BitBoard unrestricted_kings(const Position<Board>& p, bool color)
{
        return unrestricted_kings<Rules>(
                p, color, 
                Int2Type<rules::is_restricted_same_king_moves<Rules>::value>()
        );
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Rules, typename Board> 
BitBoard unrestricted_kings(const Position<Board>& p, bool color, Int2Type<false>)
{
        return p.kings(color);
}

// partial specialization for restricted consecutive moves with the same king
template<typename Rules, typename Board> 
BitBoard unrestricted_kings(const Position<Board>& p, bool color, Int2Type<true>)
{
        if (p.kings(color) && p.men(color) && is_max<Rules>(p.restricted(color).moves()))
                return p.kings(color) ^ p.restricted(color).king();
        else
                return p.kings(color);
}

template<typename Board> 
const Position<Board>* grand_parent(const Position<Board>& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<typename Board> 
const KingMoves& active_restricted(const Position<Board>& p)
{
        return p.restricted()[p.active_color()];
}

template<typename Board> 
const KingMoves& passive_restricted(const Position<Board>&);

template<typename Rules, typename Board> 
bool is_draw(const Position<Board>& p)
{
        return (
                is_repetition_draw(p) || 
                is_reversible_draw<Rules>(p)
        );
}

template<typename Board> 
bool is_repetition_draw(const Position<Board>& p)
{
        // a repetition draw needs at least 4 reversible moves
        if (p.reversible_moves() < 4)
                return false;

        // find the parent position at 4 ply above the current position
        auto q = grand_parent(*grand_parent(p));

        // compare the ancestor hash indices with the current hash index
        for (auto i = 4; i <= p.reversible_moves(); i += 2) {
                if (q->hash_index() == p.hash_index())
                        return true;
                q = grand_parent(*q);
        }
        return false;
}       

// tag dispatching on restrictions on consecutive reversible moves        
template<typename Rules, typename Board> 
bool is_reversible_draw(const Position<Board>& p)
{
        return is_reversible_draw<Rules>(
                p, 
                Int2Type<rules::is_restricted_reversible_moves<Rules>::value>()
        );
}

template<typename Rules, typename Board> 
bool is_reversible_draw(const Position<Board>&, Int2Type<false>)
{
        return false;
}

template<typename Rules, typename Board> 
bool is_reversible_draw(const Position<Board>& p, Int2Type<true>)
{
        return p.reversible_moves() >= rules::max_reversible_moves<Rules>::value;
}

namespace hash {
namespace zobrist {

// primary template
template<typename Key, typename Index>
struct Init;

// partial specialization for ab initio hashing of positions
template<typename Board, typename Index>
struct Init<Position<Board>, Index>
: 
        public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return (
                        Init<Move      , Index>()(p.material())     ^
                        Init<bool      , Index>()(p.active_color()) ^
                        Init<Restricted, Index>()(p.restricted())
                );
        }
};

}       // namespace zobrist
}       // namespace hash

}       // namespace dctl

// include template definitions inside header
#include "MakeMove.hpp"
