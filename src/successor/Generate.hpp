#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Invariant.hpp"
#include "Select.hpp"
#include "generation/Generator.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

template<typename Position>
Stack generate(Position const& p)
{
        return generate<Material::both, DefaultSelection>(p);
}

template<int Material, typename Selection, typename Position>
Stack generate(Position const& p)
{
        Stack moves;
        moves.reserve(MOVE_RESERVE);
        generate<Material, Selection>(p, moves);
        return moves;
}

template<int Material, typename Selection, typename Position>
void generate(Position const& p, Stack& moves)
{
        if (p.active_color() == Side::white)
                detail::generator<Side::white, Material, Selection, Position>()(p, moves);
        else
                detail::generator<Side::black, Material, Selection, Position>()(p, moves);
        BOOST_ASSERT((detail::invariant<Material, Selection>(p, moves.size())));
}

}       // namespace successor
}       // namespace dctl
