#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/both_jumps.hpp>
#include <dctl/successor/copy/both_promotions.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
// there are no king promotions
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/successor/copy/pawn_promotions.hpp>
#include <dctl/successor/select/conversion.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/successor/select/promotions.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct copy<Color, Material, select::conversion, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                copy<Color, Material, select::jumps, Position>()(p, moves);
                if (moves.empty())
                        copy<Color, Material, select::promotions, Position>()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
