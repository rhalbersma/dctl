#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/pawn_moves.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct detect<Color, Material::pawn, select::promotions, Position>
{
private:
        // typedefs

        typedef detect<Color, Material::pawn, select::moves, Position> PawnMoves;

public:
        bool operator()(Position const& p) const
        {
                auto const active_promotors = promoting_pawns<Color>(p);
                return active_pawns? PawnMoves().select(active_promotors, not_occupied(p)) : false;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
