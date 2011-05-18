#include "../Tree/Node/Position.h"

namespace Hash {

template<typename Index, typename Key>
Key ShiftSign<Index, Key>::operator()(Index index) const
{
        return static_cast<Key>(index >> 8 * (sizeof(Index) - sizeof(Key)));
}

template<typename Board, typename Key>
struct FindSign<Tree::Node::Position<Board>, Key>: public std::unary_function<Tree::Node::Position<Board>, Key>
{
        const Key& operator()(const Tree::Node::Position<Board>& p) const
        {
                return p.pieces();
        }
};

}       // namespace Hash
