#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/count/primary.hpp>
#include <dctl/successor/count/jumps.hpp>
#include <dctl/successor/count/both_moves.hpp>
#include <dctl/successor/count/king_moves.hpp>
#include <dctl/successor/count/pawn_moves.hpp>
#include <dctl/successor/select/legal.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/successor/select/moves.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors enumeration
template<bool Color, int Material, typename Position>
struct count<Color, Material, select::legal, Position>
{
        int operator()(Position const& p)
        {
                auto num_moves = count<Color, Material, select::jumps, Position>()(p);
                if (!num_moves)
                        num_moves += count<Color, Material, select::moves, Position>()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
