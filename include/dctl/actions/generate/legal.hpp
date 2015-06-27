#pragma once
#include <dctl/actions/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/actions/generate/jump.hpp>             // Generate (jump specialization)
#include <dctl/actions/generate/push.hpp>             // Generate (push specialization)
#include <dctl/actions/select/legal.hpp>              // legal
#include <dctl/actions/select/jump.hpp>               // jump
#include <dctl/actions/select/push.hpp>               // push
#include <dctl/player.hpp>

namespace dctl {
namespace actions {

template<Player ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Generate<ToMove, select::legal, RemoveDuplicateJumps, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& s, Sequence& moves) const
        {
                using Jump = Generate<ToMove, select::jump, RemoveDuplicateJumps, Reverse>;
                using Push = Generate<ToMove, select::push, false               , Reverse>;

                Jump{}(s, moves);
                if (moves.empty())
                        Push{}(s, moves);
        }
};

}       // namespace actions
}       // namespace dctl
