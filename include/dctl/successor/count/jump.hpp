#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/generate/all_jump.hpp>         // Generate (piece jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/move/move.hpp>                           // Move_t
#include <dctl/utility/stack_vector.hpp>                // Vector, Alloc, Arena

namespace dctl {
namespace successor {

// partial specialization for jumps
template<bool Color, class Pieces>
class Count<Color, Pieces, select::jump>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using M = Move_t<Position>;
                Arena<M> a;
                auto moves_ = stack_vector<M>(Alloc<M>{a});
                moves_.reserve(DCTL_PP_STACK_RESERVE);
                Generate<Color, Pieces, select::jump>{}(p, moves_);
                return moves_.size();
        }
};

}       // namespace successor
}       // namespace dctl
