#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/jump.hpp>                // Count (jump specialization)
#include <dctl/successor/count/push.hpp>                // Count (push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

template<Color ToMove, bool RemoveDuplicateJumps, bool Reverse>
class Count<ToMove, select::legal, RemoveDuplicateJumps, Reverse>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using Jump = Count<ToMove, select::jump, RemoveDuplicateJumps, Reverse>;
                using Push = Count<ToMove, select::push, false               , Reverse>;

                auto num_moves = Jump{}(p);
                if (!num_moves)
                        num_moves += Push{}(p);
                return num_moves;
        }
};

}       // namespace successor
}       // namespace dctl
