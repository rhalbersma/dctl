#pragma once
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/detect/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position, typename Range>
struct detector<Color, Material::both, Moves, Position, Range>
{
public:
        bool operator()(Position const& p) const
        {
                // #pieces > #kings, so the logical OR is more likely to short-circuit
                return pieces_up_moves(p) || king_down_moves(p);
        }

private:
        bool pieces_up_moves(Position const& p) const
        {
                typedef detector< Color, Material::pawn, Moves, Position, rules::range::distance_1> PiecesUpMoves;

                auto const active_pieces = p.pieces(Color);
                return active_pieces? PiecesUpMoves().select(active_pieces, not_occupied(p)) : false;
        }

        bool king_down_moves(Position const& p) const
        {
                typedef detector<!Color, Material::pawn, Moves, Position, rules::range::distance_1> KingDownMoves;

                auto const active_kings = p.kings(Color);
                return active_kings? KingDownMoves().select(active_kings, not_occupied(p)) : false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
