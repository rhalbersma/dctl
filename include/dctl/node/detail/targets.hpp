#pragma once
#include <type_traits>                  // false_type, true_type

namespace dctl {
namespace detail {

// overload for pawns that cannot capture kings
template<bool Color, class Position>
auto targets(Position const& p, std::false_type)
{
        return p.pawns(!Color);
}

// overload for pawns that can capture kings
template<bool Color, class Position>
auto targets(Position const& p, std::true_type)
{
        return p.pieces(!Color);
}

}       // namespace detail
}       // namespace dctl
