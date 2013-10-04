#pragma once
#include <dctl/node/detail/targets.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>

namespace dctl {

template<bool Color, class Position>
auto targets(Position const& p)
{
        // tag dispatching on whether pawns can capture kings
        return detail::targets<Color>(p, rules::can_jump<typename Position::rules_type, pieces::pawn, pieces::king>());
}

}       // namespace dctl
