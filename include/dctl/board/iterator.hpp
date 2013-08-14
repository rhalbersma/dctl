#pragma once
#include <dctl/angle/traits.hpp>        // is_negative, is_positive
#include <dctl/board/nested.hpp>        // shift_size
#include <dctl/board/shift.hpp>         // ShiftAssign

namespace dctl {

template<class Board, int Direction>
struct Increment
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                ShiftAssign< angle::is_positive(Direction) >()(square, board::shift_size<Board, Direction>::value);
        }
};

template<class Board, int Direction>
struct Decrement
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                ShiftAssign< angle::is_negative(Direction) >()(square, board::shift_size<Board, Direction>::value);
        }
};

template<class Board, int Direction, int N>
struct Advance
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                for (auto i = 0; i < N; ++i)
                        Increment<Board, Direction>()(square);
        }
};

template<class Board, int Direction, int N>
struct Retreat
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                for (auto i = 0; i < N; ++i)
                        Decrement<Board, Direction>()(square);
        }
};

template<class Board, int Direction, int N = 1>
struct Next
{
        template<class BidirectionalIterator>
        BidirectionalIterator operator()(BidirectionalIterator square) const
        {
                Advance<Board, Direction, N>()(square);
                return square;
        }
};

template<class Board, int Direction, int N = 1>
struct Prev
{
        template<class BidirectionalIterator>
        BidirectionalIterator operator()(BidirectionalIterator square) const
        {
                Retreat<Board, Direction, N>()(square);
                return square;
        }
};

}       // namespace dctl
