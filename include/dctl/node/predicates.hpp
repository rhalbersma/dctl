#pragma once
#include <dctl/bit/bit.hpp>
#include <dctl/node/predicates_fwd.hpp>
#include <dctl/node/detail/predicates.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/bit/algorithm.hpp>

namespace dctl {

template<class Position, class Move>
bool is_with_king(Position const& p, Move const& m)
{
        return active_kings(p).test(m.from());
}

template<class Position, class Move>
bool is_reversible(Position const& p, Move const& m)
{
        return is_with_king(p, m) && m.captured_pieces().empty() && !m.promotion();
}

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
