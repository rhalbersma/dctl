#pragma once
#include <dctl/actions/generate/primary_fwd.hpp>        // Generate (primary template)
#include <dctl/actions/generate/jump.hpp>               // Generate (jump specialization)
#include <dctl/actions/generate/push.hpp>               // Generate (push specialization)
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/color.hpp>                               // Color

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Generate<ToMove, select::legal, Unique, Reverse>
{
public:
        template<class State, class SequenceContainer>
        auto operator()(State const& state, SequenceContainer& moves) const
        {
                using Jump = Generate<ToMove, select::jump, Unique, Reverse>;
                using Push = Generate<ToMove, select::push, Unique, Reverse>;

                Jump{}(state, moves);
                if (moves.empty())
                        Push{}(state, moves);
        }
};

}       // namespace actions
}       // namespace dctl
