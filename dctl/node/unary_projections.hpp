#pragma once
#include <dctl/node/detail/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

// unoccupied squares
template<typename Position>
BitBoard not_occupied(Position const& p)
{
        return Position::board_type::squares ^ p.material().pieces();
}

// pawns for the side to move
template<typename Position>
BitBoard active_pawns(Position const& p)
{
        return p.material().pawns(active_color(p));
}

// kings for the side to move
template<typename Position>
BitBoard active_kings(Position const& p)
{
        return p.material().kings(active_color(p));
}

// pieces for the side to move
template<typename Position>
BitBoard active_pieces(Position const& p)
{
        return p.material().pieces(active_color(p));
}

// pawns for the opposite side
template<typename Position>
BitBoard passive_pawns(Position const& p)
{
        return p.material().pawns(passive_color(p));
}

// kings for the opposite side
template<typename Position>
BitBoard passive_kings(Position const& p)
{
        return p.material().kings(passive_color(p));
}

// pieces for the opposite side
template<typename Position>
BitBoard passive_pieces(Position const& p)
{
        return p.material().pieces(passive_color(p));
}

template<typename Position>
BitBoard moveable_kings(Position const& p, bool color)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return detail::moveable_kings(p, color, typename rules::traits<typename Position::rules_type>::is_restricted_same_king_moves());
}

}       // namespace dctl
