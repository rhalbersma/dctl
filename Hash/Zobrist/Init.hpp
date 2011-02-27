#include "Random.h"
#include "../../Position/Position.h"
#include "../../Position/Pieces.h"
#include "../../Position/Side.h"

namespace Hash {
namespace Zobrist {

// partial specialization for ab initio hashing of positions
template<typename Board, typename Index>
struct Init<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>, std::binary_function<Position<Board>, bool, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return (
                        Init<Pieces, Index>()(p.pieces()) ^
                        Init<bool, Index>()(p.to_move()) ^
                        Init<Position<Board>, Index>()(p, Side::BLACK) ^
                        Init<Position<Board>, Index>()(p, Side::WHITE)
                );
        }
                 
        Index operator()(const Position<Board>& p, bool color) const
        {
                return (
                        Random<Index>::xor_rand(p.repeated_kings(color), Random<Index>::REPEATED_KINGS[color]) ^
                        Random<Index>::xor_rand(p.repeated_moves(color), Random<Index>::REPEATED_MOVES[color])
                );
        }
};

// partial specialization for ab initio hashing of piece lists
template<typename Index>
struct Init<Pieces, Index>: public std::unary_function<Pieces, Index>
{
        Index operator()(const Pieces& p) const
        {
                return (
        	        Random<Index>::xor_rand(p.pieces(Side::BLACK), Random<Index>::PIECES[Side::BLACK]) ^
                        Random<Index>::xor_rand(p.pieces(Side::WHITE), Random<Index>::PIECES[Side::WHITE]) ^
                        Random<Index>::xor_rand(p.kings()            , Random<Index>::KINGS              )
                );
        }
};

// partial specialization for ab initio hashing of side to move
template<typename Index>
struct Init<bool, Index>: public std::unary_function<bool, Index>
{
        Index operator()(bool to_move) const
        {
                return Random<Index>::xor_rand(to_move, Random<Index>::SIDE);
        }

        Index operator()(void) const
        {
                return Random<Index>::SIDE;
        }
};

}       // namespace Zobrist
}       // namespace Hash
