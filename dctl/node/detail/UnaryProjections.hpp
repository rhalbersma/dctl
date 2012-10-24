#pragma once
#include <boost/mpl/bool.hpp>                   // false_, true_
#include <dctl/node/Restricted.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace detail {

// overload for unrestricted consecutive moves with the same king
template<typename Position>
BitBoard moveable_kings(Position const& p, bool color, boost::mpl::false_)
{
        return p.kings(color);
}

// overload for restricted consecutive moves with the same king
template<typename Position>
BitBoard moveable_kings(Position const& p, bool color, boost::mpl::true_)
{
        if (p.kings(color) && p.pawns(color) && is_max<typename Position::rules_type>(p.restricted(color).moves()))
                return p.kings(color) ^ p.restricted(color).king();
        else
                return p.kings(color);
}

}       // namespace detail
}       // namespace dctl
