#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/generate/all_jump.hpp>         // Generate (piece jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/move/move.hpp>                           // Move
#include <dctl/utility/stack_vector.hpp>                // Vector, Alloc, Arena

namespace dctl {
namespace successor {

// partial specialization for jumps
template<bool Color, class Pieces>
struct Count<Color, Pieces, select::jump>
{
        template<class Position>
        int operator()(Position const& p) const
        {
                using Rules = typename Position::rules_type;
                using Board = typename Position::board_type;

                Arena<Move<Rules, Board>> a;
                auto moves_ = stack_vector<Move<Rules, Board>>(Alloc<Move<Rules, Board>>{a});
                moves_.reserve(DCTL_PP_STACK_RESERVE);
                Generate<Color, Pieces, select::jump>{}(p, moves_);
                return static_cast<int>(moves_.size());
        }
};

}       // namespace successor
}       // namespace dctl
