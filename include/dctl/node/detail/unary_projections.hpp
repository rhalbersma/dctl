#pragma once
#include <type_traits>                  // false_type, true_type
#include <dctl/node/restricted.hpp>     // is_max

namespace dctl {
namespace detail {

// overload for unrestricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, bool color, std::false_type)
{
        return p.kings(color);
}

// overload for restricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, bool color, std::true_type)
{
        if (!p.kings(color).empty() && !p.pawns(color).empty() && is_max<typename Position::rules_type>(p.restricted(color).moves()))
                return p.kings(color) ^ p.restricted(color).king();
        else
                return p.kings(color);
}

}       // namespace detail
}       // namespace dctl
