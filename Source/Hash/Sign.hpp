#include "../Tree/Node/Position.h"

namespace hash {

template<typename Index, typename Key>
Key ShiftSign<Index, Key>::operator()(Index index) const
{
        return static_cast<Key>(index >> 8 * (sizeof(Index) - sizeof(Key)));
}

template<typename Board, typename Key>
struct FindSign<tree::node::Position<Board>, Key>: public std::unary_function<tree::node::Position<Board>, Key>
{
        const Key& operator()(const tree::node::Position<Board>& p) const
        {
                return p.pieces();
        }
};

}       // namespace hash
