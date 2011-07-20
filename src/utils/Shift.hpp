#include "Bit.h"
#include "../board/Traits.h"

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

template<typename Board, size_t I> template<typename T> 
T Push<Board, I>::operator()(T square) const
{
        return Shift<board::is_positive<I>::value>()(square, Board::SHIFT[I]);
}

template<typename Board, size_t I> template<typename T> 
T Pull<Board, I>::operator()(T square) const
{
        return Shift<board::is_negative<I>::value>()(square, Board::SHIFT[I]);
}

template<typename Board, size_t I> template<typename T> 
void PushAssign<Board, I>::operator()(T& square) const
{
        ShiftAssign<board::is_positive<I>::value>()(square, Board::SHIFT[I]);
}

template<typename Board, size_t I> template<typename T> 
void PullAssign<Board, I>::operator()(T& square) const
{
        ShiftAssign<board::is_negative<I>::value>()(square, Board::SHIFT[I]);
}

template<typename Board, size_t I> template<typename T> 
T FloodFill<Board, I>::operator()(T generator, T propagator) const
{
        return bit::flood_fill<board::is_positive<I>::value>(generator, propagator, Board::SHIFT[I]);
}
