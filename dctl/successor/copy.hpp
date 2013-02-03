#pragma once
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/copy/specializations.hpp>      // copy
#include <dctl/successor/invariant.hpp>                 // invariant
#include <dctl/node/material.hpp>                       // both
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/node/stack.hpp>                          // Stack

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position, typename Vector>
void copy(Position const& p, Vector& moves)
{
        detail::copy<Color, Material, Selection>()(p, moves);
}

template<bool Color, int Material, typename Selection, typename Position>
Vector<Move> copy(Position const& p, Arena<Move>& mar)
{
        Alloc<Move> mal(mar);
        Vector<Move> moves(mal);
        moves.reserve(DCTL_PP_VECTOR_RESERVE);

        copy<Color, Material, Selection>(p, moves);

        BOOST_ASSERT((invariant<Color, Material, Selection>(p, moves.size())));
        return moves;
}

template<int Material, typename Selection, typename Position>
Vector<Move> copy(Position const& p, Arena<Move>& mar)
{
        return (p.active_color() == Side::black)?
                copy<Side::black, Material, Selection>(p, mar) :
                copy<Side::white, Material, Selection>(p, mar)
        ;
}

template<typename Position>
Vector<Move> copy(Position const& p, Arena<Move>& mar)
{
        return copy<Material::both, select::legal>(p, mar);
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
