#pragma once
#include <dctl/color.hpp>
#include <dctl/position/mrp_king/mrp_king.hpp>  // is_max
#include <type_traits>                          // false_type, true_type

namespace dctl {
namespace detail {

// unrestricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, Color c, std::false_type)
{
        return p.kings(c);
}

// restricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, Color c, std::true_type)
{
        auto kings = p.kings(c);
        if (p.kings(c).any() && p.pawns(c).any() && p.mrp_king(c).is_max())
                kings.reset(p.mrp_king(c).square());
        return kings;
}

// pawns that cannot capture kings
template<Color ToMove, class Position>
auto targets(Position const& p, std::false_type)
{
        return p.pawns(!ToMove);
}

// pawns that can capture kings
template<Color ToMove, class Position>
auto targets(Position const& p, std::true_type)
{
        return p.pieces(!ToMove);
}

}       // namespace detail
}       // namespace dctl
