#pragma once
#include <dctl/angle/traits.hpp>        // is_negative, is_positive
#include <dctl/board/shift.hpp>         // shift_size, ShiftAssign

namespace dctl {

template<class Board, class Direction>
struct Increment
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                ShiftAssign< typename
                        angle::lazy::is_positive<Direction>::type,
                        shift_size<Board, Direction>
                >()(square);
        }
};

template<class Board, class Direction>
struct Decrement
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                ShiftAssign< typename
                        angle::lazy::is_negative<Direction>::type,
                        shift_size<Board, Direction>
                >()(square);
        }
};

template<class Board, class Direction, int N>
struct Advance
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                for (auto i = 0; i < N; ++i)
                        Increment<Board, Direction>()(square);
        }
};

template<class Board, class Direction, int N>
struct Retreat
{
        template<class BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                for (auto i = 0; i < N; ++i)
                        Decrement<Board, Direction>()(square);
        }
};

template<class Board, class Direction, int N = 1>
struct Next
{
        template<class BidirectionalIterator>
        BidirectionalIterator operator()(BidirectionalIterator square) const
        {
                Advance<Board, Direction, N>()(square);
                return square;
        }
};

template<class Board, class Direction, int N = 1>
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
