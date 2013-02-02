#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/count/enumerator_fwd.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/successor/copy/both_jumps.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::both, select::jumps, Position>
{
        int operator()(Position const& p) const
        {
                Arena<Move> mar_;
                Alloc<Move> mal_(mar_);
                Vector<Move> moves_(mal_);

                copy<Color, Material::both, select::jumps, Position>()(p, moves_);
                return moves_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
