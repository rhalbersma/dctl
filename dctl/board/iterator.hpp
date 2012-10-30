#pragma once
#include <dctl/angle/traits.hpp>        // is_negative, is_positive
#include <dctl/board/Shift.hpp>         // shift_size, ShiftAssign

namespace dctl {

template<typename Board, typename Direction>
struct Increment
{
        template<typename BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                ShiftAssign< typename
                        angle::lazy::is_positive<Direction>::type, typename
                        shift_size<Board, Direction>::type
                >()(square);
        }
};

template<typename Board, typename Direction>
struct Decrement
{
        template<typename BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                ShiftAssign< typename
                        angle::lazy::is_negative<Direction>::type, typename
                        shift_size<Board, Direction>::type
                >()(square);
        }
};

template<typename Board, typename Direction, int N>
struct Advance
{
        template<typename BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                for (auto i = 0; i < N; ++i)
                        Increment<Board, Direction>()(square);
        }
};

template<typename Board, typename Direction, int N>
struct Retreat
{
        template<typename BidirectionalIterator>
        void operator()(BidirectionalIterator& square) const
        {
                for (auto i = 0; i < N; ++i)
                        Decrement<Board, Direction>()(square);
        }
};

template<typename Board, typename Direction, int N = 1>
struct Next
{
        template<typename BidirectionalIterator>
        BidirectionalIterator operator()(BidirectionalIterator square) const
        {
                Advance<Board, Direction, N>()(square);
                return (square);
        }
};

template<typename Board, typename Direction, int N = 1>
struct Prev
{
        template<typename BidirectionalIterator>
        BidirectionalIterator operator()(BidirectionalIterator square) const
        {
                Retreat<Board, Direction, N>()(square);
                return (square);
        }
};

}       // namespace dctl
