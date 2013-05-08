#pragma once
#include <type_traits>                  // false_type, true_type
#include <dctl/utility/int.hpp>

namespace dctl {
namespace detail {

// overload for pawns that cannot capture kings
template<bool Color, typename Position>
BitBoard targets(Position const& p, std::false_type)
{
        return p.material().pawns(!Color);
}

// overload for pawns that can capture kings
template<bool Color, typename Position>
BitBoard targets(Position const& p, std::true_type)
{
        return p.material().pieces(!Color);
}

}       // namespace detail
}       // namespace dctl
