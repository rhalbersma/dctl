#include "Direction.h"
#include "../Utilities/Bit.h"
#include "../Utilities/Shift.h"

namespace geometry {
namespace direction {

template<typename Board, size_t Index> template<typename T> 
T Push<Board, Index>::operator()(T square) const
{
        return Shift<Traits<Index>::IS_POSITIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
T Pull<Board, Index>::operator()(T square) const
{
        return Shift<Traits<Index>::IS_NEGATIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
void PushAssign<Board, Index>::operator()(T& square) const
{
        ShiftAssign<Traits<Index>::IS_POSITIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
void PullAssign<Board, Index>::operator()(T& square) const
{
        ShiftAssign<Traits<Index>::IS_NEGATIVE>()(square, Board::DIR[Index]);
}

template<typename Board, size_t Index> template<typename T> 
T FloodFill<Board, Index>::operator()(T generator, T propagator) const
{
        return bit::flood_fill<Traits<Index>::IS_POSITIVE>(generator, propagator, Board::DIR[Index]);
}

}       // namespace direction
}       // namespace geometry
