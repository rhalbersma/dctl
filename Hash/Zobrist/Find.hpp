#include "../../Tree/Node/Position.h"

namespace Hash {
namespace Zobrist {

// partial specialization for retrieval of pre-computed indices of positions
template<typename Board, typename Index>
struct Find<Tree::Node::Position<Board>, Index>: public std::unary_function<Tree::Node::Position<Board>, Index>
{
        Index operator()(const Tree::Node::Position<Board>& p) const
        {
                return p.hash_index();
        }
};

}       // namespace Zobrist
}       // namespace Hash
