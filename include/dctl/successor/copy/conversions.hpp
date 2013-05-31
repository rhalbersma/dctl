#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/piece_jumps.hpp>          // copy (piece jumps specialization)
#include <dctl/successor/copy/piece_promotions.hpp>     // copy (piece promotions specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
// there are no king promotions
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/copy/pawn_promotions.hpp>      // copy (pawn promotions specialization)
#include <dctl/successor/select/conversions.hpp>        // conversions
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversions
template<bool Color, class Material>
struct copy<Color, Material, select::conversions>
{
        template<class Position, class Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef copy<Color, Material, select::jumps     > DoJumps;
                typedef copy<Color, Material, select::promotions> DoPromotions;

                DoJumps()(p, moves);
                if (moves.empty())
                        DoPromotions()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
