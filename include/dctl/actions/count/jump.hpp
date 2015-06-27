#pragma once
#include <dctl/player.hpp>                               // Player
#include <dctl/action/action.hpp>                           // Action_t
#include <dctl/actions/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/actions/generate/jump.hpp>             // Generate (jump specialization)
#include <dctl/actions/select/jump.hpp>               // jump
#include <dctl/utility/stack_vector.hpp>                // Vector, Alloc, Arena

namespace dctl {
namespace actions {

template<Player ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Count<ToMove, select::jump, RemoveDuplicateJumps, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                using M = Action_t<State>;
                Arena<M> a;
                auto moves = stack_vector<M>(Alloc<M>{a});
                moves.reserve(DCTL_PP_STACK_RESERVE);
                Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>{}(p, moves);
                return moves.size();
        }
};

}       // namespace actions
}       // namespace dctl
