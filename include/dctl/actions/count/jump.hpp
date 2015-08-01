#pragma once
#include <dctl/color.hpp>                       // Player
#include <dctl/action/action.hpp>               // Action_t
#include <dctl/actions/count/primary_fwd.hpp>   // Count (primary template)
#include <dctl/actions/generate/jump.hpp>       // Generate (jump specialization)
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/utility/type_traits.hpp>                 // rules_type, board_type
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
                using Action_t = Action<rules_type_t<State>, board_type_t<State>>;
                //static std::vector<Action_t> moves;
                //moves.clear();
                util::bounded_vector<Action_t, 64> moves;
                Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>{}(p, moves);
                return moves.size();
        }
};

}       // namespace actions
}       // namespace dctl
