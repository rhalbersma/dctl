#include "Bit.h"
#include "Shift.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"

template<typename Board, size_t Index> template<typename T> 
T Push<Board, Index>::operator()(T square) const
{
        return Shift<DirTraits<Index>::IS_POSITIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
T Pull<Board, Index>::operator()(T square) const
{
        return Shift<DirTraits<Index>::IS_NEGATIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
void PushAssign<Board, Index>::operator()(T& square) const
{
        ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
void PullAssign<Board, Index>::operator()(T& square) const
{
        ShiftAssign<DirTraits<Index>::IS_NEGATIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
T FloodFill<Board, Index>::operator()(T generator, T propagator) const
{
        return Bit::flood_fill<DirTraits<Index>::IS_POSITIVE>(generator, propagator, Board::DIR[Index]);
}
