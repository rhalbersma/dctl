#include "Random.h"
#include "../../Tree/Node/Position.h"
#include "../../Tree/Node/Pieces.h"
#include "../../Tree/Node/Side.h"

namespace hash {
namespace zobrist {

// partial specialization for ab initio hashing of positions
template<typename Board, typename Index>
struct Init<tree::node::Position<Board>, Index>: public std::unary_function<tree::node::Position<Board>, Index>, std::binary_function<tree::node::Position<Board>, bool, Index>
{
        Index operator()(const tree::node::Position<Board>& p) const
        {
                return (
                        Init<tree::node::Pieces, Index>()(p.pieces()) ^
                        Init<bool, Index>()(p.to_move()) ^
                        Init<tree::node::Position<Board>, Index>()(p, tree::node::Side::BLACK) ^
                        Init<tree::node::Position<Board>, Index>()(p, tree::node::Side::WHITE)
                );
        }
                 
        Index operator()(const tree::node::Position<Board>& p, bool color) const
        {
                return (
                        Random<Index>::xor_rand(p.same_king(color)      , Random<Index>::SAME_KING[color]      ) ^
                        Random<Index>::xor_rand(p.same_king_moves(color), Random<Index>::SAME_KING_MOVES[color])
                );
        }
};

// partial specialization for ab initio hashing of piece lists
template<typename Index>
struct Init<tree::node::Pieces, Index>: public std::unary_function<tree::node::Pieces, Index>
{
        Index operator()(const tree::node::Pieces& p) const
        {
                return (
        	        Random<Index>::xor_rand(p.pieces(tree::node::Side::BLACK), Random<Index>::PIECES[tree::node::Side::BLACK]) ^
                        Random<Index>::xor_rand(p.pieces(tree::node::Side::WHITE), Random<Index>::PIECES[tree::node::Side::WHITE]) ^
                        Random<Index>::xor_rand(p.kings()                        , Random<Index>::KINGS                          )
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

}       // namespace zobrist
}       // namespace hash
