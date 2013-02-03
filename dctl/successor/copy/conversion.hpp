#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/both_jumps.hpp>           // copy (combined king and pawn jumps specialization)
#include <dctl/successor/copy/both_promotions.hpp>      // copy (combined king and pawn promotions specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
// there are no king promotions
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/copy/pawn_promotions.hpp>      // copy (pawn promotions specialization)
#include <dctl/successor/select/conversion.hpp>         // conversion
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material>
struct copy<Color, Material, select::conversion>
{
        template<typename Position, typename Vector>
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
