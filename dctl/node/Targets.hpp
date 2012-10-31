#pragma once
#include <dctl/node/detail/Targets.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {

template<bool Color, typename Position>
BitBoard targets(Position const& p)
{
        // tag dispatching on whether pawns can capture kings
        return (detail::targets<Color>(p, typename Position::rules_type::is_pawns_jump_kings()));
}

}       // namespace dctl
