#pragma once
#include <dctl/color.hpp>
#include <dctl/position/detail/unary_projections.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>
#include <dctl/board/mask.hpp>

namespace dctl {

// pieces for the side to move
template<class Position>
auto active_pieces(Position const& p)
{
        return p.pieces(p.to_move());
}

// kings for the side to move
template<class Position>
auto active_kings(Position const& p)
{
        return p.kings(p.to_move());
}

// pawns for the side to move
template<class Position>
auto active_pawns(Position const& p)
{
        return p.pawns(p.to_move());
}

// pieces for the opposite side
template<class Position>
auto passive_pieces(Position const& p)
{
        return p.pieces(!p.to_move());
}

// kings for the opposite side
template<class Position>
auto passive_kings(Position const& p)
{
        return p.kings(!p.to_move());
}

// pawns for the opposite side
template<class Position>
auto passive_pawns(Position const& p)
{
        return p.pawns(!p.to_move());
}

template<class Position>
auto moveable_kings(Position const& p, Color c)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return detail::moveable_kings(p, c, is_restricted_same_king_push_t<rules_type_t<Position>>{});
}

template<Color ToMove, class Position>
auto targets(Position const& p)
{
        // tag dispatching on whether pawns can capture kings
        return detail::targets<ToMove>(p, is_pawn_jump_king_t<rules_type_t<Position>>{});
}

}       // namespace dctl
