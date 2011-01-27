#include "../Position/Position.h"

namespace Hash {

template<typename Index, typename Key>
Key ShiftSign<Index, Key>::operator()(Index index) const
{
        return static_cast<Key>(index >> 8 * (sizeof(Index) - sizeof(Key)));
}

template<typename Board, typename Key>
struct FindSign<Position<Board>, Key>: public std::unary_function<Position<Board>, Key>
{
        const Key& operator()(const Position<Board>& p) const
        {
                return p.pieces();
        }
};

}       // namespace Hash