#include "Random.h"
#include "../../Tree/Node/Position.h"
#include "../../Tree/Node/Pieces.h"
#include "../../Tree/Node/Side.h"

namespace Hash {
namespace Zobrist {

// partial specialization for ab initio hashing of positions
template<typename Board, typename Index>
struct Init<Tree::Node::Position<Board>, Index>: public std::unary_function<Tree::Node::Position<Board>, Index>, std::binary_function<Tree::Node::Position<Board>, bool, Index>
{
        Index operator()(const Tree::Node::Position<Board>& p) const
        {
                return (
                        Init<Tree::Node::Pieces, Index>()(p.pieces()) ^
                        Init<bool, Index>()(p.to_move()) ^
                        Init<Tree::Node::Position<Board>, Index>()(p, Tree::Node::Side::BLACK) ^
                        Init<Tree::Node::Position<Board>, Index>()(p, Tree::Node::Side::WHITE)
                );
        }
                 
        Index operator()(const Tree::Node::Position<Board>& p, bool color) const
        {
                return (
                        Random<Index>::xor_rand(p.repeated_kings(color), Random<Index>::REPEATED_KINGS[color]) ^
                        Random<Index>::xor_rand(p.repeated_moves(color), Random<Index>::REPEATED_MOVES[color])
                );
        }
};

// partial specialization for ab initio hashing of piece lists
template<typename Index>
struct Init<Tree::Node::Pieces, Index>: public std::unary_function<Tree::Node::Pieces, Index>
{
        Index operator()(const Tree::Node::Pieces& p) const
        {
                return (
        	        Random<Index>::xor_rand(p.pieces(Tree::Node::Side::BLACK), Random<Index>::PIECES[Tree::Node::Side::BLACK]) ^
                        Random<Index>::xor_rand(p.pieces(Tree::Node::Side::WHITE), Random<Index>::PIECES[Tree::Node::Side::WHITE]) ^
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

}       // namespace Zobrist
}       // namespace Hash
