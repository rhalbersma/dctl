#pragma once
#include <dctl/node/detail/UnaryProjections.hpp>
#include <dctl/node/BinaryProjections.hpp>
#include <dctl/bit/Bit.hpp>
#include <dctl/rules/Enum.hpp>

namespace dctl {
namespace detail {

// overload for apres-fini capture removal
template<typename Position, typename Move>
bool is_intersecting_capture(Position const& /* p */, Move const& /* m */, rules::removal::apres_fini)
{
        return false;
}

// overload for en-passant capture removal
template<typename Position, typename Move>
bool is_intersecting_capture(Position const& p, Move const& m, rules::removal::en_passant)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_pawns(p));
}

}       // namespace detail
}       // namespace dctl
