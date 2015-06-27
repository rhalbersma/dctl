#pragma once
#include <dctl/player.hpp>                               // Player
#include <dctl/actions/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/actions/count/jump.hpp>                // Count (jump specialization)
#include <dctl/actions/count/push.hpp>                // Count (push specialization)
#include <dctl/actions/select/legal.hpp>              // legal
#include <dctl/actions/select/jump.hpp>               // jump
#include <dctl/actions/select/push.hpp>               // push

namespace dctl {
namespace actions {

template<Player ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Count<ToMove, select::legal, RemoveDuplicateJumps, Reverse>
{
public:
        template<class State>
        auto operator()(State const& p) const
        {
                using Jump = Count<ToMove, select::jump, RemoveDuplicateJumps, Reverse>;
                using Push = Count<ToMove, select::push, false               , Reverse>;

                auto num_moves = Jump{}(p);
                if (!num_moves)
                        num_moves += Push{}(p);
                return num_moves;
        }
};

}       // namespace actions
}       // namespace dctl
