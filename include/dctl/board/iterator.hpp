#pragma once
#include <dctl/angle/angle.hpp>
#include <dctl/angle/traits.hpp>        // is_negative, is_positive
#include <dctl/utility/shift.hpp>       // shift_assign

namespace dctl {

template<class Board, int Direction>
struct Increment
{
        template<class BidirectionalIterator, int N = Board::shift_size(Angle{Direction})>
        void operator()(BidirectionalIterator& square) const
        {
                util::shift_assign< angle::is_positive(Direction) >()(square, N);
        }
};

template<class Board, int Direction>
struct Decrement
{
        template<class BidirectionalIterator, int N = Board::shift_size(Angle{Direction})>
        void operator()(BidirectionalIterator& square) const
        {
                util::shift_assign< angle::is_negative(Direction) >()(square, N);
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
