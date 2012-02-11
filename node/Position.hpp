#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <functional>                           // std::unary_function
#include "Move.hpp"
#include "Restricted.hpp"
#include "Side.hpp"
#include "Predicates_fwd.hpp"
#include "../bit/Bit.hpp"
#include "../hash/zobrist/Init.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Board> 
struct Position
{
public:
        /*
        Position() 
        { 
                // no-op
        }
*/
        // initialize with a set of bitboards and a color
        Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
        :
                parent_(nullptr),
                material_(black_pieces, white_pieces, kings),
                reversible_moves_(0),
                distance_to_root_(0),
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

        const Move& key() const
        {
                return material();
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

        PlyCount distance_to_root() const
        {
                return distance_to_root_;
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
        template<typename Rules> 
        void copy_make(const Position<Board>& other, const Move& move)
        {
                *this = other;          // copy the position
                attach(other);          // attach the position
                make<Rules>(move);      // make the move
        }
        
        template<typename Rules> 
        void make(const Move& m)
        {
                BOOST_ASSERT(is_pseudo_legal<Rules>(*this, m));

                make_irreversible<Rules>(m);
                make_incremental(m);

                BOOST_ASSERT(material_invariant());
                BOOST_ASSERT(hash_index_invariant());
        }
        
        void attach(const Position<Board>& other)
        {
                parent_ = &other;       // link the pointers
        }
        
private:
        // implementation
        void detach();

        template<typename Rules> 
        void make_irreversible(const Move& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible<Rules>(
                        m, 
                        Int2Type<rules::is_restricted_same_king_moves<Rules>::value>()
                );
        }
        
        // partial specialization for restricted consecutive moves with the same king
        template<typename Rules> 
        void make_irreversible(const Move& m, Int2Type<true>)
        {
                make_irreversible<Rules>(m, Int2Type<false>());
                make_restricted<Rules>(m);
        }
        
        // partial specialization for unrestricted consecutive moves with the same king
        template<typename Rules> 
        void make_irreversible(const Move& m, Int2Type<false>)
        {
                make_reversible_moves(m);
                make_distance_to_root();
        }

        void make_reversible_moves(const Move& m)
        {
                if (is_reversible(*this, m))
                        ++reversible_moves_;
                else
                        reversible_moves_ = 0;
        }
        
        void make_distance_to_root()
        {
                ++distance_to_root_;
        }

        template<typename Rules> 
        void make_restricted(const Move& m)
        {
                make_active_king_moves<Rules>(m);
                make_passive_king_moves<Rules>(m);                
        }
        
        template<typename Rules> 
        void make_active_king_moves(const Move& m)
        {
                typedef hash::zobrist::Init<KingMoves, HashIndex> Hash;
                KingMoves& restricted = restricted_[active_color()];

                if (active_kings(*this) && active_men(*this)) {
                        hash_index_ ^= Hash()(restricted, active_color());
                        if (is_reversible(*this, m) && !is_max<Rules>(restricted.moves())) {
                                if (restricted.king() & from_sq(*this, m))
                                        restricted.increment(dest_sq(*this, m));               
                                else
                                        restricted.init(dest_sq(*this, m));
                                hash_index_ ^= Hash()(restricted, active_color());
                        } else
                                restricted.reset();
                } 
        }
        
        template<typename Rules> 
        void make_passive_king_moves(const Move& m)
        {
                typedef hash::zobrist::Init<KingMoves, HashIndex> Hash;
                KingMoves& restricted = restricted_[passive_color()];

                if (
                        restricted.moves() && is_capture(*this, m) &&
                        (
                                bit::is_subset_of(restricted.king() , captured_pieces(*this, m)) ||
                                bit::is_subset_of(passive_men(*this), captured_pieces(*this, m))
                        )
                ) {
                        hash_index_ ^= Hash()(restricted, passive_color());
                        restricted.reset();
                }
        }
        
        void make_incremental(const Move& m)
        {
                make_material(m);
                make_to_move();
        }

        void make_material(const Move& m)
        {
                material_ ^= m;
                hash_index_ ^= hash::zobrist::Init<Move, HashIndex>()(m);
        }

        void make_to_move()
        {
                to_move_ ^= Side::pass;
                hash_index_ ^= hash::zobrist::Init<bool, HashIndex>()(Side::pass);
        }
                
        // post-conditions for the constructors and modifiers
        bool material_invariant() const
        {
                return (
                        material_.invariant() &&
                        bit::is_subset_of(pieces(), Board::squares)
                );
        }

        bool hash_index_invariant() const
        {
                return hash_index_ == hash::zobrist::Init<Position<Board>, HashIndex>()(*this);
        }

        // representation
        const Position<Board>* parent_;
        HashIndex hash_index_; 
        Move material_;
        Restricted restricted_;
        PlyCount reversible_moves_;
        PlyCount distance_to_root_;
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

template<typename Rules, typename Board> 
BitBoard unrestricted_kings(const Position<Board>& p, bool color)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return unrestricted_kings_dispatch<Rules>(
                p, color, 
                Int2Type<rules::is_restricted_same_king_moves<Rules>::value>()
        );
}

// partial specialization for unrestricted consecutive moves with the same king
template<typename Rules, typename Board> 
BitBoard unrestricted_kings_dispatch(const Position<Board>& p, bool color, Int2Type<false>)
{
        return p.kings(color);
}

// partial specialization for restricted consecutive moves with the same king
template<typename Rules, typename Board> 
BitBoard unrestricted_kings_dispatch(const Position<Board>& p, bool color, Int2Type<true>)
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
