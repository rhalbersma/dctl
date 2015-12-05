#pragma once
#include <dctl/action/action.hpp>               // Action
#include <dctl/actions/count/primary_fwd.hpp>   // Count (primary template)
#include <dctl/actions/generate/jump.hpp>       // Generate (jump specialization)
#include <dctl/actions/select/jump.hpp>         // jump
#include <dctl/color.hpp>                       // Color
#include <dctl/utility/type_traits.hpp>         // rules_t, board_t
#include <boost/container/static_vector.hpp>

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Count<ToMove, select::jump, Unique, Reverse>
{
public:
        template<class State>
        auto operator()(State const& state) const
        {
                boost::container::static_vector<Action<rules_t<State>, board_t<State>>, 64> moves;
                Generate<ToMove, select::jump, Unique, Reverse>{}(state, moves);
                return moves.size();
        }
};

}       // namespace actions
}       // namespace dctl
