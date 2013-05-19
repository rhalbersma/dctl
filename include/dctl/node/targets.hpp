#pragma once
#include <dctl/node/detail/targets.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

template<bool Color, typename Position>
BitBoard targets(Position const& p)
{
        // tag dispatching on whether pawns can capture kings
        return detail::targets<Color>(p, rules::is_pawns_jump_kings<typename Position::rules_type>());
}

}       // namespace dctl
