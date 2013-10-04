#pragma once
#include <dctl/node/detail/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {

// unoccupied squares
template<class Position>
auto not_occupied(Position const& p)
{
        return Position::board_type::squares ^ p.material().pieces();
}

// pawns for the side to move
template<class Position>
auto active_pawns(Position const& p)
{
        return p.material().pawns(active_color(p));
}

// kings for the side to move
template<class Position>
auto active_kings(Position const& p)
{
        return p.material().kings(active_color(p));
}

// pieces for the side to move
template<class Position>
auto active_pieces(Position const& p)
{
        return p.material().pieces(active_color(p));
}

// pawns for the opposite side
template<class Position>
auto passive_pawns(Position const& p)
{
        return p.material().pawns(passive_color(p));
}

// kings for the opposite side
template<class Position>
auto passive_kings(Position const& p)
{
        return p.material().kings(passive_color(p));
}

// pieces for the opposite side
template<class Position>
auto passive_pieces(Position const& p)
{
        return p.material().pieces(passive_color(p));
}

template<class Position>
auto moveable_kings(Position const& p, bool color)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return detail::moveable_kings(p, color, rules::is_restricted_same_king_moves<typename Position::rules_type>());
}

}       // namespace dctl
