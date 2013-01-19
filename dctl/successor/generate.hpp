#pragma once
#include <dctl/successor/select.hpp>                    // DefaultSelection
#include <dctl/successor/generation/generator.hpp>      // generator
#include <dctl/successor/generation/invariant.hpp>      // invariant
#include <dctl/node/material.hpp>                       // both
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/node/stack.hpp>                          // Stack

namespace dctl {
namespace successor {

template<int Material, typename Selection, typename Position>
Vector<Move> generate(Position const& p, Arena<Move>& mar)
{
        Alloc<Move> mal(mar);
        Vector<Move> moves(mal);
        moves.reserve(DCTL_PP_VECTOR_RESERVE);

        if (p.active_color() == Side::black)
                detail::generator<Side::black, Material, Selection, Position>{moves}(p);
        else
                detail::generator<Side::white, Material, Selection, Position>{moves}(p);

        BOOST_ASSERT((detail::invariant<Material, Selection>(p, moves.size())));
        return moves;
}

template<typename Position>
Vector<Move> generate(Position const& p, Arena<Move>& mar)
{
        return generate<Material::both, DefaultSelection>(p, mar);
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
