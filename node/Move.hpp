#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Side.hpp"
#include "../bit/Bit.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

struct Move
{
public:
        Move() 
        { 
                /* no-op */ 
        }

        // zero initialize
        explicit Move(BitBoard /* MUST be zero */)
        {
                init<Side::black>(0, 0, 0);
                BOOST_ASSERT(invariant());
        }        

        // initialize with a set of bitboards
        Move(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                BOOST_ASSERT(invariant());
        }        

        // create from a set of bitboards
        template<bool Color>
        static Move create(BitBoard active_pieces, BitBoard passive_pieces, BitBoard kings)
        {
	        Move temp;
                temp.init<Color>(active_pieces, passive_pieces, kings);
                return temp;
        }

        // logical consistency of the representation
        bool invariant() const
        {
                return (
		        bit::is_exclusive(pieces(Side::black), pieces(Side::white)) &&
                        bit::is_within(kings(), pieces())
                );
        }

        // black or white men
        BitBoard men(bool color) const
        {
                return pieces(color) & ~kings();
        }
        
        // black or white kings
        BitBoard kings(bool color) const
        {
                return pieces(color) & kings();
        }                     
        
        // black or white pieces
        BitBoard pieces(bool color) const
        {
                return pieces_[color];
        }                    
        
        // black and white men
        BitBoard men() const
        {
                return pieces() & ~kings();
        }                           
        
        // black and white kings
        BitBoard kings() const
        {
                return kings_;
        }                         
        
        // black and white pieces
        BitBoard pieces() const
        {
	        return pieces(Side::black) ^ pieces(Side::white);
        }                        

        // xor-assign the set bits of another piece set
        Move& operator^=(const Move& other)
        {
                pieces_[Side::black] ^= other.pieces(Side::black);
                pieces_[Side::white] ^= other.pieces(Side::white);
                kings_ ^= other.kings();
                return *this;
        }          

private:
        // initialize with a set of bitboards
        template<bool Color> 
        void init(BitBoard active_pieces, BitBoard passive_pieces, BitBoard kings)
        {
	        pieces_[ Color] = active_pieces;
	        pieces_[!Color] = passive_pieces;
                kings_ = kings;
        }                

        // representation
        BitBoard pieces_[2];    // black and white pieces
        BitBoard kings_;        // kings
};

// xor the set bits of two piece sets
inline const Move operator^(const Move& left, const Move& right)
{
        return Move(left) ^= right;
}     

inline bool operator==(const Move& left, const Move& right)
{
        return (
		(left.pieces(Side::black) == right.pieces(Side::black)) &&
		(left.pieces(Side::white) == right.pieces(Side::white)) &&
                            (left.kings() == right.kings())
        );
}

inline bool operator!=(const Move& left, const Move& right)
{
        return !(left == right);
}

}       // namespace dctl
