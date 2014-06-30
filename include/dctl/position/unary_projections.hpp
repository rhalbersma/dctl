#pragma once
#include <dctl/position/detail/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/board/mask.hpp>

namespace dctl {

// pieces for the side to move
template<class Position>
auto active_pieces(Position const& p)
{
        return p.pieces(p.active_color());
}

// kings for the side to move
template<class Position>
auto active_kings(Position const& p)
{
        return p.kings(p.active_color());
}

// pawns for the side to move
template<class Position>
auto active_pawns(Position const& p)
{
        return p.pawns(p.active_color());
}

// pieces for the opposite side
template<class Position>
auto passive_pieces(Position const& p)
{
        return p.pieces(!p.active_color());
}

// kings for the opposite side
template<class Position>
auto passive_kings(Position const& p)
{
        return p.kings(!p.active_color());
}

// pawns for the opposite side
template<class Position>
auto passive_pawns(Position const& p)
{
        return p.pawns(!p.active_color());
}

template<class Position>
auto moveable_kings(Position const& p, bool color)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return detail::moveable_kings(p, color, rules::is_restricted_same_king_moves<typename Position::rules_type>{});
}

template<bool Color, class Position>
auto targets(Position const& p)
{
        // tag dispatching on whether pawns can capture kings
        return detail::targets<Color>(p, is_pawn_jump_king_t<typename Position::rules_type>{});
}

}       // namespace dctl
