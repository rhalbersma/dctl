#include "../node/Position.h"

namespace tree {
namespace hash {
namespace zobrist {

// partial specialization for retrieval of pre-computed indices of positions
template<typename Board, typename Index>
struct Find<Position<Board>, Index>: public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return p.hash_index();
        }
};

}       // namespace zobrist
}       // namespace hash
}       // namespace tree