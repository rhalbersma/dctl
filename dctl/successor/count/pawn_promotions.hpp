#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/count/pawn_moves.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct count<Color, Material::pawn, select::promotions, Position>
{
private:
        // typedefs

        typedef count<Color, Material::pawn, select::moves, Position> PawnMoves;

public:
        int operator()(Position const& p) const
        {
                auto const active_promotors = promoting_pawns<Color>(p);
                return active_promotors? PawnMoves().select(active_promotors, not_occupied(p)) : 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
