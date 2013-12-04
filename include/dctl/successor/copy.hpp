#pragma once
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/copy/specializations.hpp>      // copy
#include <dctl/successor/invariant.hpp>                 // invariant
#include <dctl/pieces/pieces.hpp>                       // pawn, king, piece
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/utility/stack_vector.hpp>                // stack_vector

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position, class Sequence>
void copy(Position const& p, Sequence& moves)
{
        detail::copy<Color, Pieces, Select>{}(p, moves);
}

template<bool Color, class Pieces, class Select, class Position, class Allocator>
auto copy(Position const& p, Allocator const& alloc)
{
        using Move = typename Allocator::value_type;
        stack_vector<Move> moves(alloc);
        moves.reserve(DCTL_PP_STACK_RESERVE);

        copy<Color, Pieces, Select>(p, moves);

        assert((invariant<Color, Pieces, Select>(p, static_cast<int>(moves.size()))));
        return moves;
}

template<class Pieces, class Select, class Position, class Allocator>
auto copy(Position const& p, Allocator const& alloc)
{
        return (p.to_move() == Side::black) ?
                copy<Side::black, Pieces, Select>(p, alloc) :
                copy<Side::white, Pieces, Select>(p, alloc)
        ;
}

template<class Position, class Allocator>
auto copy(Position const& p, Allocator const& alloc)
{
        return copy<pieces::all, select::legal>(p, alloc);
}

template<class Position, class Move>
auto make_copy(Position const& p, Move const& m)
{
        auto q = p;
        q.attach(p);
        q.make(m);
        return q;
}

}       // namespace successor
}       // namespace dctl
