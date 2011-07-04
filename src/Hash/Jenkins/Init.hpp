#include "Mix.h"
#include "../../Tree/Node/Position.h"
#include "../../Tree/Node/Side.h"

namespace hash {
namespace jenkins {

template<typename Board, typename Index>
struct Init<tree::node::Position<Board>, Index>: public std::unary_function<tree::node::Position<Board>, Index>
{
        Index operator()(const tree::node::Position<Board>& p) const
        {
                Index index_ = 0;

                Mix<Index>::mix(index_, p.to_move());
                Mix<Index>::mix(index_, p.pieces(tree::node::Side::BLACK));
                Mix<Index>::mix(index_, p.pieces(tree::node::Side::WHITE));
                Mix<Index>::mix(index_, p.kings());
                
                return index_;
        }
};

}       // namespace jenkins
}       // namespace hash
