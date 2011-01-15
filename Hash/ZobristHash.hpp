#include "ZobristRandom.h"
#include "../Position/Position.h"
#include "../Position/Pieces.h"
#include "../Position/Side.h"

// partial specialization for retrieval of pre-computed indices of positions
template<typename Board, typename Index>
struct ZobristFind<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return p.hash_index();
        }
};

// partial specialization for ab initio hashing of positions
template<typename Board, typename Index>
struct ZobristHash<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>, std::binary_function<Position<Board>, bool, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return (
                        ZobristHash<Pieces, Index>()(p.pieces()) ^
                        ZobristHash<bool, Index>()(p.to_move()) ^
                        ZobristHash<Position<Board>, Index>()(p, Side::BLACK) ^
                        ZobristHash<Position<Board>, Index>()(p, Side::WHITE)
                );
        }
                 
        Index operator()(const Position<Board>& p, bool color) const
        {
                return (
                        ZobristRandom<Index>::xor_rand(p.repeated_kings(color), ZobristRandom<Index>::REPEATED_KINGS[color]) ^
                        ZobristRandom<Index>::xor_rand(p.repeated_moves(color), ZobristRandom<Index>::REPEATED_MOVES[color])
                );
        }
};

// partial specialization for ab initio hashing of piece lists
template<typename Index>
struct ZobristHash<Pieces, Index>: public std::unary_function<Pieces, Index>
{
        Index operator()(const Pieces& p) const
        {
                return (
        	        ZobristRandom<Index>::xor_rand(p.pieces(Side::BLACK), ZobristRandom<Index>::PIECES[Side::BLACK]) ^
                        ZobristRandom<Index>::xor_rand(p.pieces(Side::WHITE), ZobristRandom<Index>::PIECES[Side::WHITE]) ^
                        ZobristRandom<Index>::xor_rand(p.kings()            , ZobristRandom<Index>::KINGS              )
                );
        }
};

// partial specialization for ab initio hashing of side to move
template<typename Index>
struct ZobristHash<bool, Index>: public std::unary_function<bool, Index>
{
        Index operator()(bool to_move) const
        {
                return ZobristRandom<Index>::xor_rand(to_move, ZobristRandom<Index>::SIDE);
        }

        Index operator()(void) const
        {
                return ZobristRandom<Index>::SIDE;
        }
};
