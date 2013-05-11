#pragma once
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/copy/specializations.hpp>      // copy
#include <dctl/successor/invariant.hpp>                 // invariant
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/node/move.hpp>                           // Move
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/utility/stack_vector.hpp>                // stack_vector

namespace dctl {
namespace successor {

template<bool Color, typename Material, typename Select, typename Position, typename Vector>
void copy(Position const& p, Vector& moves)
{
        detail::copy<Color, Material, Select>()(p, moves);
}

template<bool Color, typename Material, typename Select, typename Position>
stack_vector<Move>::type copy(Position const& p, Arena<Move>::type& mar)
{
        Alloc<Move>::type mal(mar);
        stack_vector<Move>::type moves(mal);
        moves.reserve(DCTL_PP_STACK_RESERVE);

        copy<Color, Material, Select>(p, moves);

        BOOST_ASSERT((invariant<Color, Material, Select>(p, static_cast<int>(moves.size()))));
        return moves;
}

template<typename Material, typename Select, typename Position>
stack_vector<Move>::type copy(Position const& p, Arena<Move>::type& mar)
{
        return (p.to_move() == Side::black)?
                copy<Side::black, Material, Select>(p, mar) :
                copy<Side::white, Material, Select>(p, mar)
        ;
}

template<typename Position>
stack_vector<Move>::type copy(Position const& p, Arena<Move>::type& mar)
{
        return copy<material::piece, select::legal>(p, mar);
}

template<typename Position, typename Move>
Position make_copy(Position const& p, Move const& m)
{
        auto q = p;
        q.attach(p);
        q.make(m);
        return q;
}

}       // namespace successor
}       // namespace dctl
