#pragma once
#include <type_traits>                          // false_type, true_type
#include <dctl/position/mru_king/mru_king.hpp>  // is_max

namespace dctl {
namespace detail {

// unrestricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, bool color, std::false_type)
{
        return p.kings(color);
}

// restricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, bool color, std::true_type)
{
        auto kings = p.kings(color);
        if (!p.kings(color).empty() && !p.pawns(color).empty() && p.mru_king(color).is_max())
                kings.reset(p.mru_king(color).square());
        return kings;
}

// pawns that cannot capture kings
template<bool Color, class Position>
auto targets(Position const& p, std::false_type)
{
        return p.pawns(!Color);
}

// pawns that can capture kings
template<bool Color, class Position>
auto targets(Position const& p, std::true_type)
{
        return p.pieces(!Color);
}

}       // namespace detail
}       // namespace dctl
