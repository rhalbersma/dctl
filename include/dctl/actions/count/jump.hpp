#pragma once
#include <dctl/color.hpp>                       // Player
#include <dctl/action/action.hpp>               // Action_t
#include <dctl/actions/count/primary_fwd.hpp>   // Count (primary template)
#include <dctl/actions/generate/jump.hpp>       // Generate (jump specialization)
#include <dctl/actions/select/jump.hpp>         // jump
#include <vector>                               // vector
#include <dctl/utility/bounded_vector.hpp>

namespace dctl {
namespace actions {

template<Color ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Count<ToMove, select::jump, RemoveDuplicateJumps, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                //static std::vector<Action_t<State>> moves;
                //moves.clear();
                util::bounded_vector<Action_t<State>, 64> moves;
                Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>{}(p, moves);
                return moves.size();
        }
};

}       // namespace actions
}       // namespace dctl
