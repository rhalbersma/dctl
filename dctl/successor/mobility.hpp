#pragma once
#include <cstddef>                                      // size_t
#include <dctl/successor/count.hpp>                     // count
#include <dctl/successor/material/piece.hpp>            // both
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {

template<bool Color, typename Material, typename Select, typename Position>
std::size_t mobility(Position const& p)
{
        return count<Color, Material, Select>(p);
}

template<bool Color, typename Position>
std::size_t mobility(Position const& p)
{
        return count<Color, material::piece, select::moves>(p);
}

}       // namespace successor
}       // namespace dctl
