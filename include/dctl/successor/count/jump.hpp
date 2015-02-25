#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/move/move.hpp>                           // Move_t
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/generate/jump.hpp>             // Generate (jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/utility/stack_vector.hpp>                // Vector, Alloc, Arena

namespace dctl {
namespace successor {

template<Color ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Count<ToMove, select::jump, RemoveDuplicateJumps, Reverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using M = Move_t<Position>;
                Arena<M> a;
                auto moves = stack_vector<M>(Alloc<M>{a});
                moves.reserve(DCTL_PP_STACK_RESERVE);
                Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>{}(p, moves);
                return moves.size();
        }
};

}       // namespace successor
}       // namespace dctl
