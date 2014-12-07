#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/successor/count/primary_fwd.hpp>         // Count (primary template)
#include <dctl/successor/count/all_jump.hpp>            // Count (jump specialization)
#include <dctl/successor/count/all_push.hpp>            // Count (push specialization)
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/select/push.hpp>               // push

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse>
class Count<ToMove, IsReverse, select::legal>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using Jump = Count<ToMove, IsReverse, select::jump>;
                using Push = Count<ToMove, IsReverse, select::push>;

                auto num_moves = Jump{}(p);
                if (!num_moves)
                        num_moves += Push{}(p);
                return num_moves;
        }
};

}       // namespace successor
}       // namespace dctl
