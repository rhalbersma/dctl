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
        return generate<Material::both, Legal>(p);
}

template<int Material, typename Selection, typename Position>
Stack generate(Position const& p)
{
        Stack moves;
        moves.reserve(MOVE_RESERVE);

        if (p.active_color() == Side::white)
                detail::generator<Side::white, Material, Selection, Position>()(p, moves);
        else
                detail::generator<Side::black, Material, Selection, Position>()(p, moves);
        BOOST_ASSERT((detail::invariant<Material, Selection>(p, moves.size())));

        return moves;
}

}       // namespace successor
}       // namespace dctl
