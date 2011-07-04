#include "Bit.h"
#include "../board/Board.h"
#include "../board/Direction.h"

namespace { enum { L, R }; }

// specialization for bitwise shift-left
template<>
struct Shift<L>
{
        template<typename T> 
        T operator()(T square, size_t dir) const
        {
                return square << dir;
        }
};

// specialization for bitwise shift-right
template<>
struct Shift<R>
{
        template<typename T> 
        T operator()(T square, size_t dir) const
        {
                return square >> dir;
        }
};

// specialization for bitwise shift-left assignment
template<>
struct ShiftAssign<L>
{
        template<typename T> 
        void operator()(T& square, size_t dir) const
        {
                square <<= dir;
        }
};

// specialization for bitwise shift-right assignment
template<>
struct ShiftAssign<R>
{
        template<typename T> 
        void operator()(T& square, size_t dir) const
        {
                square >>= dir;
        }
};

template<typename Board, size_t Index> template<typename T> 
T Push<Board, Index>::operator()(T square) const
{
        return Shift<board::direction::Traits<Index>::IS_POSITIVE>()(square, Board::DIRECTION[Index]);
}

template<typename Board, size_t Index> template<typename T> 
T Pull<Board, Index>::operator()(T square) const
{
        return Shift<board::direction::Traits<Index>::IS_NEGATIVE>()(square, Board::DIRECTION[Index]);
}

template<typename Board, size_t Index> template<typename T> 
void PushAssign<Board, Index>::operator()(T& square) const
{
        ShiftAssign<board::direction::Traits<Index>::IS_POSITIVE>()(square, Board::DIRECTION[Index]);
}

template<typename Board, size_t Index> template<typename T> 
void PullAssign<Board, Index>::operator()(T& square) const
{
        ShiftAssign<board::direction::Traits<Index>::IS_NEGATIVE>()(square, Board::DIRECTION[Index]);
}

template<typename Board, size_t Index> template<typename T> 
T FloodFill<Board, Index>::operator()(T generator, T propagator) const
{
        return bit::flood_fill<board::direction::Traits<Index>::IS_POSITIVE>(generator, propagator, Board::DIRECTION[Index]);
}
