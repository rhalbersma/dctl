#include "Mix.h"
#include "../../Tree/Node/Position.h"
#include "../../Tree/Node/Side.h"

namespace Hash {
namespace Jenkins {

template<typename Board, typename Index>
struct Init<Tree::Node::Position<Board>, Index>: public std::unary_function<Tree::Node::Position<Board>, Index>
{
        Index operator()(const Tree::Node::Position<Board>& p) const
        {
                Index index_ = 0;

                Mix<Index>::mix(index_, p.to_move());
                Mix<Index>::mix(index_, p.pieces(Tree::Node::Side::BLACK));
                Mix<Index>::mix(index_, p.pieces(Tree::Node::Side::WHITE));
                Mix<Index>::mix(index_, p.kings());
                
                return index_;
        }
};

}       // namespace Jenkins
}       // namespace Hash
