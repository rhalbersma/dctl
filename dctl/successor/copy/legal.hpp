#pragma once
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/primary.hpp>
#include <dctl/successor/copy/both_jumps.hpp>
#include <dctl/successor/copy/both_moves.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
#include <dctl/successor/copy/king_moves.hpp>
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/successor/copy/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/capture/state.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<bool Color, int Material, typename Position>
struct generator<Color, Material, Legal, Position>
{
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                generator<Color, Material, Jumps, Position>()(p, moves);
                if (moves.empty())
                        generator<Color, Material, Moves, Position>()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
