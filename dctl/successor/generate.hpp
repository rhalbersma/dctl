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
Stack generate(Position const& p, MoveArena& mar)
{
        MoveAlloc mal(mar);
        Stack moves(mal);
        moves.reserve(DCTL_PP_VECTOR_RESERVE);

        if (p.active_color() == Side::black)
                detail::generator<Side::black, Material, Selection, Position>{moves}(p);
        else
                detail::generator<Side::white, Material, Selection, Position>{moves}(p);

        BOOST_ASSERT((detail::invariant<Material, Selection>(p, moves.size())));
        return moves;
}

template<typename Position>
Stack generate(Position const& p, MoveArena& mar)
{
        return generate<Material::both, DefaultSelection>(p, mar);
}

}       // namespace successor
}       // namespace dctl
