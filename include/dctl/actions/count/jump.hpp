#pragma once
#include <dctl/color.hpp>                       // Player
#include <dctl/action/action.hpp>               // Action
#include <dctl/actions/count/primary_fwd.hpp>   // Count (primary template)
#include <dctl/actions/generate/jump.hpp>       // Generate (jump specialization)
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/utility/type_traits.hpp>         // rules_t, board_t
#include <dctl/utility/bounded_vector.hpp>
#include <vector>                               // vector

namespace dctl {
namespace actions {

template<Color ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Count<ToMove, select::jump, RemoveDuplicateJumps, Reverse>
{
public:
        template<class State>
        auto operator()(State const& s) const
        {
                util::bounded_vector<Action<rules_t<State>, board_t<State>>, 64> moves;
                //static std::vector<Action<rules_t<State>, board_t<State>>> moves;
                //moves.clear();
                Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>{}(s, moves);
                return moves.size();
        }
};

}       // namespace actions
}       // namespace dctl
