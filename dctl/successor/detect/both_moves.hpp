#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/impl/pawn_moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/node/material.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position, typename Range>
struct detect<Color, Material::both, select::moves, Position, Range>
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
                typedef impl::detect< Color, Material::pawn, select::moves, Position, rules::range::distance_1> PiecesUpMoves;

                auto const active_pieces = p.pieces(Color);
                return active_pieces? PiecesUpMoves().select(active_pieces, not_occupied(p)) : false;
        }

        bool king_down_moves(Position const& p) const
        {
                typedef impl::detect<!Color, Material::pawn, select::moves, Position, rules::range::distance_1> KingDownMoves;

                auto const active_kings = p.kings(Color);
                return active_kings? KingDownMoves().select(active_kings, not_occupied(p)) : false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
