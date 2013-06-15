#pragma once
#include <type_traits>                  // false_type, true_type
#include <dctl/node/restricted.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace detail {

// overload for unrestricted consecutive moves with the same king
template<class Position>
BitBoard moveable_kings(Position const& p, bool color, std::false_type)
{
        return p.material().kings(color);
}

// overload for restricted consecutive moves with the same king
template<class Position>
BitBoard moveable_kings(Position const& p, bool color, std::true_type)
{
        if (p.material().kings(color) && p.material().pawns(color) && is_max<typename Position::rules_type>(p.restricted(color).moves()))
                return p.material().kings(color) ^ p.restricted(color).king();
        else
                return p.material().kings(color);
}

}       // namespace detail
}       // namespace dctl
