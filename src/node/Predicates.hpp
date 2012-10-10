#pragma once
#include "Predicates_fwd.hpp"
#include "UnaryProjections.hpp"
#include "BinaryProjections.hpp"
#include "detail/Predicates.hpp"
#include "../bit/Bit.hpp"

namespace dctl {

template<typename Position, typename Move>
bool is_connected(Position const& p, Move const& m1, Move const& m2)
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
        return detail::is_intersecting_capture(p, m, typename Position::rules_type::jump_removal());
}

template<typename Position, typename Move>
bool is_pseudo_legal(Position const& p, Move const& m)
{
        return (
                // cannot move multiple pieces
                !(bit::is_multiple(from_sq(p, m)) || bit::is_multiple(dest_sq(p, m))) &&

                // only capture existing pieces
                bit::is_subset_of(captured_pieces(p, m), passive_pieces(p)) &&
                (
                        // only capture existing kings
                        bit::is_subset_of(captured_kings(p, m), passive_kings(p)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture(p, m)
                )
        );
}

}       // namespace dctl
