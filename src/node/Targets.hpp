#pragma once
#include "detail/Targets.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<bool Color, typename Position>
BitBoard targets(Position const& p)
{
        // tag dispatching on whether pawns can capture kings
        return detail::targets<Color>(p, typename Position::rules_type::is_pawns_jump_kings());
}

}       // namespace dctl
