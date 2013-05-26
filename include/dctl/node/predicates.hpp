#pragma once
#include <dctl/bit/bit.hpp>
#include <dctl/node/predicates_fwd.hpp>
#include <dctl/node/binary_projections.hpp>
#include <dctl/node/detail/predicates.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/packed/algorithm.hpp>

namespace dctl {

template<typename Position, typename Move>
bool is_connected(Position const& /* p */, Move const& /* m1 */, Move const& /* m2 */)
{
        return false;
}

template<typename Position, typename Move>
bool is_promotion(Position const& p, Move const& m)
{
        return bit::is_single(moving_kings(p, m));
}

template<typename Position, typename Move>
bool is_with_king(Position const& p, Move const& m)
{
        return !bit::is_zero(moving_kings(p, m) & active_kings(p));
}

template<typename Position, typename Move>
bool is_capture(Position const& p, Move const& m)
{
        return !bit::is_zero(captured_pieces(p, m));
}

template<typename Position, typename Move>
bool is_reversible(Position const& p, Move const& m)
{
        return is_with_king(p, m) && !is_capture(p, m);
}

template<typename Position, typename Move>
bool is_intersecting_capture(Position const& p, Move const& m)
{
        // tag dispatching on capture removal
        return detail::is_intersecting_capture(p, m, rules::phase_capture<typename Position::rules_type>());
}

template<typename Position, typename Move>
bool is_pseudo_legal(Position const& p, Move const& m)
{
        return (
                // cannot move multiple pieces
                !(bit::is_multiple(from_sq(p, m)) || bit::is_multiple(dest_sq(p, m))) &&

                // only capture existing pieces
                packed::set_includes(passive_pieces(p), captured_pieces(p, m)) &&
                (
                        // only capture existing kings
                        packed::set_includes(passive_kings(p), captured_kings(p, m)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture(p, m)
                )
        );
}

}       // namespace dctl
