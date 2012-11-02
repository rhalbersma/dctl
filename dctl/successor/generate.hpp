#pragma once
#include <dctl/successor/select.hpp>                 // DefaultSelection
#include <dctl/successor/generation/generate.hpp>    // generator
#include <dctl/node/material.hpp>                    // both
#include <dctl/node/stack.hpp>                       // Stack

namespace dctl {
namespace successor {

template<int Material, typename Selection, typename Position>
Stack generate(Position const& p)
{
        Stack moves;

        // parentheses around function objects to avoid "C++'s most vexing parse"
        (detail::generate<Material, Selection, Position>(moves))(p);
        
        return (moves);
}

template<typename Position>
Stack generate(Position const& p)
{
        return (generate<Material::both, DefaultSelection>(p));
}

}       // namespace successor
}       // namespace dctl