#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<bool Color, template<typename, typename> class Position, typename Rules, typename Board>
static BitBoard targets(Position<Rules, Board> const& p)
{
        // tag dispatching on whether pawns can capture kings
        return detail::targets<Color>(p, typename Rules::is_pawns_jump_kings());
}

namespace detail {

// overload for pawns that can capture kings
template<bool Color, typename Position>
static BitBoard targets(Position const& p, boost::mpl::true_)
{
        return p.pieces(!Color);
}

// overload for pawns that cannot capture kings
template<bool Color, typename Position>
static BitBoard targets(Position const& p, boost::mpl::false_)
{
        return p.pawns(!Color);
}

}       // namespace detail
}       // namespace dctl