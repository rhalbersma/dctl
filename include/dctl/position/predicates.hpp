#pragma once
#include <dctl/bit.hpp>
#include <dctl/position/detail/predicates.hpp>
#include <dctl/position/predicates_fwd.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {

template<class Position, class Move>
bool is_intersecting_capture(Position const& p, Move const& m)
{
        // tag dispatching on capture removal
        return detail::is_intersecting_capture(p, m, rules::phase::capture<typename Position::rules_type>());
}

template<class Position, class Move>
bool is_pseudo_legal(Position const& p, Move const& m)
{
        return (
                // only capture existing pieces
                bit::set_includes(passive_pieces(p), m.captured_pieces()) &&
                (
                        // only capture existing kings
                        bit::set_includes(passive_kings(p), m.captured_kings(m)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture(p, m)
                )
        );
}

}       // namespace dctl
