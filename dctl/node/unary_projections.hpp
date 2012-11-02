#pragma once
#include <dctl/node/detail/unary_projections.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

// unoccupied squares
template<typename Position>
BitBoard not_occupied(Position const& p)
{
        return (Position::board_type::squares ^ p.pieces());
}

// pawns for the side to move
template<typename Position>
BitBoard active_pawns(Position const& p)
{
        return (p.pawns(p.active_color()));
}

// kings for the side to move
template<typename Position>
BitBoard active_kings(Position const& p)
{
        return (p.kings(p.active_color()));
}

// pieces for the side to move
template<typename Position>
BitBoard active_pieces(Position const& p)
{
        return (p.pieces(p.active_color()));
}

// pawns for the opposite side
template<typename Position>
BitBoard passive_pawns(Position const& p)
{
        return (p.pawns(p.passive_color()));
}

// kings for the opposite side
template<typename Position>
BitBoard passive_kings(Position const& p)
{
        return (p.kings(p.passive_color()));
}

// pieces for the opposite side
template<typename Position>
BitBoard passive_pieces(Position const& p)
{
        return (p.pieces(p.passive_color()));
}

template<typename Position>
BitBoard moveable_kings(Position const& p, bool color)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return (detail::moveable_kings(p, color, typename Position::rules_type::is_restricted_same_king_moves()));
}

}       // namespace dctl
