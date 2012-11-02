#pragma once
#include <boost/mpl/bool.hpp>                   // false_, true_
#include <dctl/utility/int.hpp>

namespace dctl {
namespace detail {

// overload for pawns that cannot capture kings
template<bool Color, typename Position>
BitBoard targets(Position const& p, boost::mpl::false_)
{
        return (p.pawns(!Color));
}

// overload for pawns that can capture kings
template<bool Color, typename Position>
BitBoard targets(Position const& p, boost::mpl::true_)
{
        return (p.pieces(!Color));
}

}       // namespace detail
}       // namespace dctl
