#pragma once
#include <dctl/bit/predicates.hpp>
#include <dctl/node/detail/unary_projections.hpp>
#include <dctl/node/binary_projections.hpp>
#include <dctl/rules/types.hpp>

namespace dctl {
namespace detail {

// overload for apres-fini capture removal
template<class Position, class Move>
bool is_intersecting_capture(Position const& /* p */, Move const& /* m */, rules::phase::apres_fini)
{
        return false;
}

// overload for en-passant capture removal
template<class Position, class Move>
bool is_intersecting_capture(Position const& p, Move const& m, rules::phase::en_passant)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single((moving_kings(p, m) & captured_kings(p, m) & passive_pawns(p)).as_block());
}

}       // namespace detail
}       // namespace dctl
