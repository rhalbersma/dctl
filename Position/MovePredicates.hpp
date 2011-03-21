#include "../Utilities/Bit.h"

template<typename Board>
bool  Position<Board>::is_connected(const Pieces& m1, const Pieces& m2) const
{
        // two-step advance of single piece
        if (dest_sq(m1) == from_sq(m2))
                return true;

        // second piece fills square vacated by first piece 
        if (from_sq(m1) == dest_sq(m2))
                return true;

        return false;
}

template<typename Board>
bool Position<Board>::is_non_conversion(const Pieces& m) const
{
        return is_with_king(m) && !is_capture(m);
}

template<typename Board>
bool Position<Board>::is_promotion(const Pieces& m) const
{
        return Bit::is_single(m.kings(to_move()));
}

template<typename Board>
bool Position<Board>::is_with_king(const Pieces& m) const
{
        return (kings(to_move()) & from_sq(m)) != 0;
}

template<typename Board>
bool Position<Board>::is_capture(const Pieces& m) const
{
        return m.pieces(!to_move()) != 0;
}

template<typename Board>
BitBoard Position<Board>::from_sq(const Pieces& m) const
{
        return pieces(to_move()) & m.pieces(to_move());
}

template<typename Board>
BitBoard Position<Board>::dest_sq(const Pieces& m) const
{
        return not_occupied() & m.pieces(to_move());
}
