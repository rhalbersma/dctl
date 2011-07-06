#include "../../Tree/Node/Position.h"

namespace hash {
namespace zobrist {

// partial specialization for retrieval of pre-computed indices of positions
template<typename Board, typename Index>
struct Find<tree::node::Position<Board>, Index>: public std::unary_function<tree::node::Position<Board>, Index>
{
        Index operator()(const tree::node::Position<Board>& p) const
        {
                return p.hash_index();
        }
};

}       // namespace zobrist
}       // namespace hash
